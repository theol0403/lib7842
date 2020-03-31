#include "lib7842/api/purePursuit/pathFollower.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QArea.hpp"
#include "pros/rtos.hpp"
#include <iostream>
#include <utility>

namespace lib7842 {

PathFollower::PathFollower(std::shared_ptr<ChassisModel> imodel,
                           std::shared_ptr<Odometry> iodometry, const ChassisScales& ichassisScales,
                           const QAngularSpeed& igearset, const QLength& ilookahead,
                           const QLength& idriveRadius) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  chassisScales(ichassisScales),
  gearset(igearset),
  lookahead(ilookahead),
  driveRadius(idriveRadius) {}

void PathFollower::followPath(const PursuitPath& ipath, bool ibackwards,
                              const std::optional<QSpeed>& istartSpeed) {
  resetPursuit();

  auto rate = global::getTimeUtil()->getRate();
  auto timer = global::getTimeUtil()->getTimer();

  PursuitLimits limits = ipath.getLimits();
  // assume the robot starts at minimum velocity unless otherwise specified
  QSpeed lastVelocity = istartSpeed.value_or(limits.minVel);

  auto& path = ipath(); // simplify getting path

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

    // if the robot is on the path, use the normal lookahead. If not, use the projected.
    auto& finalLook = onPath ? lookPoint : projectedLook;

    // whether the robot is within the driveRadius of the end of the path. If it is, disable angle
    // correction.
    bool withinDriveRadius = Vector::dist(lookPoint, *path.back()) < driveRadius &&
                             Vector::dist(pos, *path.back()) < driveRadius &&
                             Vector::dist(**closest, *path.back()) < driveRadius;

    // calculate the arc curvature for the robot to travel to the lookahead
    double curvature = withinDriveRadius ? 0 : calculateCurvature(pos, finalLook);

    // the angle to the end of the path
    QAngle angleToEnd = pos.angleTo(*path.back()).abs();

    // we are done the path if the angle is opposite of the drive direction
    bool pastEnd = ibackwards ? angleToEnd < 90_deg : angleToEnd > 90_deg;

    // if within the the of the path, ignore the default parameter and drive directly to the end. We
    // are past the end of the path if the angle is above 90, so drive backwards if so.
    if (withinDriveRadius) ibackwards = angleToEnd > 90_deg;

    // the robot is considered finished if it has passed the end
    isFinished = pastEnd && withinDriveRadius;

    // if the robot is on the path, choose the lowest of either the path velocity or the
    // curvature-based speed reduction. If the robot is not on the path, choose the lowest of either
    // the max velocity or the curvature-based speed reduction.
    QSpeed targetVel = 0_mps;
    if (onPath) {
      auto pathSpeed = closest->get()->getData<QSpeed>("velocity");
      targetVel =
        limits.k ? std::min(pathSpeed, limits.k.value() / std::abs(curvature)) : pathSpeed;
    } else {
      targetVel =
        limits.k ? std::min(limits.maxVel, limits.k.value() / std::abs(curvature)) : limits.maxVel;
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

    double left = (wheelVel[0] / gearset).convert(number);
    double right = (wheelVel[1] / gearset).convert(number);

    // normalize the sides
    double maxMag = std::max(std::abs(left), std::abs(right));
    if (maxMag > 1.0) {
      left /= maxMag;
      right /= maxMag;
    }

    if (ibackwards) {
      left *= -1;
      right *= -1;
    }

    if (mode == util::motorMode::voltage) {
      model->tank(left, right);
    } else {
      model->left(left);
      model->right(right);
    }

    rate->delayUntil(10_ms);
  }

  model->driveVector(0, 0); // apply velocity braking
}

void PathFollower::setMotorMode(util::motorMode imode) {
  mode = imode;
}

PathFollower::pathIterator_t PathFollower::findClosest(const PursuitPath& ipath,
                                                       const Vector& ipos) {
  auto& path = ipath(); // simplify getting path

  QLength closestDist {std::numeric_limits<double>::max()};
  // get the last closest point, or the beginning of the path if there is none
  auto closest = lastClosest.value_or(path.begin());

  // Optimization: limit the progression of the closest point. It considers the last closest point,
  // and all the options up to one point ahead of the lookahead. This improves performance and
  // prevents the robot from skipping ahead on the path.
  //
  // If the end of the path is within the lookahead, it considers all options up to the end of the
  // path. This is to make sure that the closest point is the end in the scenario where the
  // lookahead is not the end of the path.

  auto end =
    Vector::dist(ipos, *path.back()) > lookahead ? path.begin() + lastLookIndex + 2 : path.end();

  // loop from the last closest point to one point past the lookahead
  for (auto it = closest; it < end; it++) {
    if (it >= path.end()) break;
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
  auto& path = ipath(); // simplify getting path

  // Optimization: if the robot starts within the end of the path, then the only intersection is
  // behind the robot, causing the robot to drive backwards when we want it to go straight to the
  // lookahead. To fix this, if the lookahead has not been found yet, but the robot is within the
  // end of the path, then jump the lookahead to the end of the path.
  if (lastClosest && Vector::dist(ipos, *path.back()) < lookahead) {
    lastClosest = path.end() - 2;
    lastLookT = 1;
  }

  // lookahead intersection should not be behind closest
  size_t lastClosestIndex = lastClosest.value_or(path.begin()) - path.begin();

  // used for optimizing number of intersection searches
  size_t lastIntersect = 0;

  // loop through every segment looking for intersection
  for (size_t i = std::max(lastLookIndex, lastClosestIndex); i < path.size() - 1; i++) {
    auto& start = *path[i];
    auto& end = *path[i + 1];

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
    if (lastIntersect > 0 && Vector::dist(*path[i], *path[lastIntersect]) >= lookahead * 2) {
      break;
    }
  }

  const auto& start = *path[lastLookIndex];
  const auto& end = *path[lastLookIndex + 1];
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
    if (t2 >= 0.0 && t2 <= 1.0) { return t2; }
    if (t1 >= 0.0 && t1 <= 1.0) { return t1; }
  }

  // no intersection on this interval
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
