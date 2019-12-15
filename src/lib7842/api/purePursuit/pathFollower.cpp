#include "pathFollower.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QArea.hpp"
#include "pros/rtos.hpp"
#include <iostream>

namespace lib7842 {

PathFollower::PathFollower(const std::shared_ptr<ChassisModel>& imodel,
                           const std::shared_ptr<Odometry>& iodometry,
                           const ChassisScales& ichassisScales, const QLength& ilookahead,
                           const TimeUtil& itimeUtil) :
  model(imodel),
  odometry(iodometry),
  chassisScales(ichassisScales),
  lookahead(ilookahead),
  timeUtil(itimeUtil) {}

void PathFollower::followPath(const PursuitPath& ipath) {
  resetPursuit();

  auto rate = timeUtil.getRate();
  auto timer = timeUtil.getTimer();

  PursuitLimits limits = ipath.getLimits();
  QSpeed lastVelocity = limits.minVel; // assume the robot starts at minimum velocity

  bool isFinished = false; // loop until the robot is considered to have finished the path
  while (!isFinished) {
    // get the robot position and heading
    State pos = State(odometry->getState(StateMode::CARTESIAN));

    auto closest = findClosest(ipath, pos); // get an iterator to the closest point
    Vector lookPoint = findLookaheadPoint(ipath, pos);
    // the robot is on the path if the distance to the closest point is smaller than the lookahead
    bool onPath = Vector::dist(pos, **closest) <= lookahead;

    // project the lookahead point onto the lookahead radius. When the lookahead point is further
    // than the lookahead radius, this can cause some problems with the robot curvature calculation.
    // The projected point will cause the robot to rotate more appropriately.
    Vector projectedLook = (MathPoint::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

    // calculate the arc curvature for the robot to travel to the lookahead
    double curvature = calculateCurvature(pos, projectedLook);

    // the robot is considered finished if the closest point is the end of the path
    isFinished = closest >= ipath().end() - 1;

    // if the robot is on the path, choose the lowest of either the path velocity or the
    // curvature-based speed reduction. If the robot is not on the path, choose the lowest of either
    // the max velocity or the curvature-based speed reduction.
    QSpeed targetVel = 0_mps;
    if (onPath) {
      targetVel =
        std::min(closest->get()->getData<QSpeed>("velocity"), limits.k / std::abs(curvature));
    } else {
      targetVel = std::min(limits.maxVel, limits.k / std::abs(curvature));
    }

    // add an upwards rate limiter to the robot velocity using the formula vf=vi+at
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    auto dT = timer->getDt();
    // get maximum allowable change in velocity
    QSpeed maxVelocity = lastVelocity + dT * limits.accel;
    // limit the velocity
    if (targetVel > maxVelocity) targetVel = maxVelocity;
    lastVelocity = targetVel;

    // calculate robot wheel velocities
    auto wheelVel = calculateVelocity(targetVel, curvature, chassisScales, limits);

    // model->left(leftWheel.convert(rpm) / 200);
    // model->right(rightWheel.convert(rpm) / 200);
    model->tank(wheelVel[0].convert(rpm) / 200, wheelVel[1].convert(rpm) / 200);

    rate->delayUntil(10_ms);
  }

  model->tank(0, 0);
}

PathFollower::pathIterator_t PathFollower::findClosest(const PursuitPath& ipath,
                                                       const Vector& ipos) {
  QLength closestDist {std::numeric_limits<double>::max()};
  // get the last closest point, or the beginning of the path if there is none
  auto closest = lastClosest.value_or(ipath().begin());

  // Optimization: limit the progression of the closest point. It considers the last closest point,
  // and all the options up to one point ahead of the lookahead. This improves performance and
  // prevents the robot from skipping ahead on the path.
  //
  // If the end of the path is within the lookahead, it considers all options up to the end of the
  // path. This is to make sure that the closest point is the end in the scenario where the
  // lookahead is not the end of the path.

  auto end = Vector::dist(ipos, *ipath().back()) > lookahead ? ipath().begin() + lastLookIndex + 2
                                                             : ipath().end();

  // loop from the last closest point to one point past the lookahead
  for (auto it = closest; it < end; it++) {
    if (it >= ipath().end()) break;
    QLength distance = Vector::dist(ipos, **it);
    if (distance < closestDist) {
      closestDist = distance;
      closest = it;
    }
  }

  lastClosest = closest;
  return closest;
}

Vector PathFollower::findLookaheadPoint(const PursuitPath& ipath, const Vector& ipos) {
  // used for optimizing number of intersection searches
  size_t lastIntersect = 0;

  // lookahead intersection should not be behind closest
  size_t lastClosestIndex = lastClosest.value_or(ipath().begin()) - ipath().begin();

  // loop through every segment looking for intersection
  for (size_t i = std::max(lastLookIndex, lastClosestIndex); i < ipath().size() - 1; i++) {
    auto& start = *ipath()[i];
    auto& end = *ipath()[i + 1];

    auto t = findIntersectT(start, end, ipos, lookahead);
    if (t) {
      // If the segment is further along or the fractional index is greater, then this is the
      // correct point
      if (i > lastLookIndex || t.value() > lastLookT) {
        lastLookIndex = i;
        lastLookT = t.value();
        // if this is the second intersection that was found, we are done
        if (lastIntersect > 0) break;
        // record the index of the first intersection
        lastIntersect = i;
      }
    }

    // Optimization: if an intersection has been found, and the loop is checking distances from the
    // last intersection that are further than the lookahead, we are done.
    if (lastIntersect > 0 && Vector::dist(*ipath()[i], *ipath()[lastIntersect]) >= lookahead * 2) {
      break;
    }
  }

  auto& start = *ipath()[lastLookIndex];
  auto& end = *ipath()[lastLookIndex + 1];
  return start + ((end - start) * lastLookT);
}

std::optional<double> PathFollower::findIntersectT(const Vector& start, const Vector& end,
                                                   const Vector& pos, const QLength& lookahead) {
  Vector d = end - start;
  Vector f = start - pos;

  double a = MathPoint::dot(d, d);
  double b = MathPoint::dot(d, f) * 2.0;
  double c = MathPoint::dot(f, f) - std::pow(lookahead.convert(meter), 2);
  double dis = std::pow(b, 2) - (4.0 * (a * c));

  if (dis >= 0) {
    dis = std::sqrt(dis);
    double t1 = (-b - dis) / (2.0 * a);
    double t2 = (-b + dis) / (2.0 * a);

    // prioritize further down path
    if (t2 >= 0.0 && t2 <= 1.0) {
      return t2;
    } else if (t1 >= 0.0 && t1 <= 1.0) {
      return t1;
    }
  }

  //no intersection on this interval
  return std::nullopt;
}

double PathFollower::calculateCurvature(const State& state, const Vector& lookPoint) {
  MathPoint pos(state);
  MathPoint look(lookPoint);
  MathPoint diff = look - pos;
  double head = ((state.theta * -1) + 90_deg).convert(radian);
  double a = -std::tan(head);
  double c = -a * pos.x - pos.y;
  double x = std::abs(a * look.x + 1.0 * look.y + c) / std::sqrt(std::pow(a, 2) + 1);
  int side = util::sgn(std::sin(head) * diff.x - std::cos(head) * diff.y);
  double curv = (2.0 * x) / std::pow(MathPoint::dist(state, lookPoint), 2);
  return std::pow(curv, 2) * side;
}

std::valarray<QAngularSpeed> PathFollower::calculateVelocity(const QSpeed& vel, double curvature,
                                                             const ChassisScales& chassisScales,
                                                             const PursuitLimits& limits) {
  QSpeed leftVel = vel * (2.0 + chassisScales.wheelTrack.convert(meter) * curvature) / 2.0;
  QSpeed rightVel = vel * (2.0 - chassisScales.wheelTrack.convert(meter) * curvature) / 2.0;

  leftVel = std::clamp(leftVel, limits.maxVel * -1, limits.maxVel);
  rightVel = std::clamp(rightVel, limits.maxVel * -1, limits.maxVel);

  QAngularSpeed leftWheel = (leftVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;
  QAngularSpeed rightWheel = (rightVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;

  return {leftWheel, rightWheel};
}

void PathFollower::resetPursuit() {
  lastClosest = std::nullopt;
  lastLookIndex = 0;
  lastLookT = 0;
}

} // namespace lib7842