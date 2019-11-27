#include "pathFollower.hpp"

namespace lib7842 {

PathFollower::PathFollower(const std::shared_ptr<ChassisModel>& imodel,
                           const std::shared_ptr<Odometry>& iodometry,
                           const QLength& ilookahead) :
  model(imodel), odometry(iodometry), lookahead(ilookahead) {}

void PathFollower::followPath(const PursuitPath& ipath) {

  // reset pursuit
  lastClosest = std::nullopt;
  lastLookIndex = 0;
  lastLookT = 0;

  // get the pursuit limits
  PursuitLimits limits = ipath.getLimits();
  // get the starting position
  State lastPos = State(odometry->getState(StateMode::CARTESIAN));
  // assume the robot starts at minimum velocity
  QSpeed lastVelocity = limits.minVel;

  // loop until the robot is considered to have finished the path
  bool isFinished = false;
  while (!isFinished) {
    // get the robot state
    State pos = State(odometry->getState(StateMode::CARTESIAN));

    // get an iterator to the closest point
    auto closest = findClosest(ipath, pos);
    std::cout << "Close: " << closest - ipath().begin() << ", ";
    // get the lookahead point
    Vector lookPoint = findLookaheadPoint(ipath, pos);
    // the robot is on the path if the distance to the closest point is smaller than the lookahead
    bool onPath = Vector::dist(pos, **closest) < lookahead;
    std::cout << "On: " << onPath << ", ";

    // project the lookahead point onto the lookahead radius. When the lookahead point is further
    // than the lookahead radius, this can cause some problems with the robot curvature calculation.
    // The projected point will cause the robot to rotate more appropriately.
    Vector projectedLookPoint =
      (MathPoint::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

    // use the normal lookahead point if the robot is on the path and the distance to the lookahead
    // is smaller than the distance to the projected point
    Vector& finalLookPoint =
      onPath && Vector::dist(pos, lookPoint) < Vector::dist(pos, projectedLookPoint)
        ? lookPoint
        : projectedLookPoint;

    // calculate the curvature in order for the robot to arc to the lookahead
    QCurvature curv = calculateCurvature(pos, finalLookPoint);
    std::cout << "Curv: " << curv.convert(curvature) << ", ";

    // the robot is considered finished if it is on the path, the closest point is the end of the
    // path, and the lookahead is the end of the path
    isFinished = onPath && (closest >= ipath().end() - 1) && lastLookIndex >= ipath().size() - 2;
    std::cout << "Done " << isFinished << ", ";

    // if the robot is on the path, choose the lowest of either the path velocity or the
    // curvature-based speed reduction. If the robot is not on the path, choose the lowest of either
    // the max velocity or the curvature-based speed reduction.
    QSpeed targetVel = 0_mps;
    if (onPath) {
      targetVel = mps * std::min(closest->get()->getData<QSpeed>("velocity").convert(mps),
                                 limits.k / std::abs(curv.convert(curvature)));
    } else {
      targetVel =
        mps * std::min(limits.maxVel.convert(mps), limits.k / std::abs(curv.convert(curvature)));
    }

    // add an upwards rate limiter to the robot velocity. Assume the robot starts at the minimum
    // velocity and prevent the robot from going slower than it. Calculate the distance travelled
    // since the last calculation and calculate maximum change in velocity acording to acceleration.
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    // get distance traveled since last calculation
    QLength distDt = Vector::dist(lastPos, pos);
    // get maximum allowable change in velocity
    QSpeed maxVelocity = mps * std::sqrt(std::pow(lastVelocity.convert(mps), 2) +
                                         (2 * limits.accel.convert(mps2) * distDt.convert(meter)));
    // limit the velocity
    if (targetVel > maxVelocity) targetVel = maxVelocity;

    lastPos = pos;
    lastVelocity = targetVel;

    std::cout << "Vel: " << targetVel.convert(mps) << ", ";

    // calculate robot wheel velocities
    auto robotVel = calculateVelocity(targetVel, curv, odometry->getScales().wheelTrack);
    auto leftVel = std::clamp(robotVel[0], -limits.maxVel, limits.maxVel);
    auto rightVel = std::clamp(robotVel[1], -limits.maxVel, limits.maxVel);

    // convert to rpm
    QAngularSpeed leftWheel = (leftVel / (1_pi * odometry->getScales().wheelDiameter)) * 360_deg;
    QAngularSpeed rightWheel = (rightVel / (1_pi * odometry->getScales().wheelDiameter)) * 360_deg;

    std::cout << "Left: " << leftWheel.convert(rpm) << ", ";
    std::cout << "Right: " << rightWheel.convert(rpm) << std::endl;
    model->left(leftWheel.convert(rpm) / 200);
    model->right(rightWheel.convert(rpm) / 200);

    pros::delay(10);
  }

  model->tank(0, 0);
}

PursuitPath::array_t::const_iterator PathFollower::findClosest(const PursuitPath& ipath,
                                                               const Vector& ipos) {
  QLength closestDist {std::numeric_limits<double>::max()};
  // get the last closest point, or the beginning of the path if there is none
  auto closest = lastClosest.value_or(ipath().begin());

  // Optimization: limit the progression of the closest point. It considers the last closest point,
  // and all the options up to one point ahead of the lookahead. This improves performance and
  // prevents the robot from skipping ahead on the path.

  // loop from the last closest point to one point past the lookahead
  for (auto it = closest; it <= ipath().begin() + lastLookIndex + 1; it++) {
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

  // loop through every segment looking for intersection
  for (size_t i = lastLookIndex; i < ipath().size() - 1; i++) {
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
    if (lastIntersect > 0 && Vector::dist(*ipath()[i], *ipath()[lastIntersect]) >= lookahead) {
      break;
    }
  }

  auto& start = *ipath()[lastLookIndex];
  auto& end = *ipath()[lastLookIndex + 1];
  return start + ((end - start) * lastLookT);
}

std::optional<double> PathFollower::findIntersectT(const Vector& ifirst,
                                                   const Vector& isecond,
                                                   const Vector& ipos,
                                                   const QLength& ilookahead) {
  Vector d = isecond - ifirst;
  Vector f = ifirst - ipos;

  double a = MathPoint::dot(d, d);
  double b = 2.0 * MathPoint::dot(f, d);
  double c = MathPoint::dot(f, f) - (ilookahead * ilookahead).convert(meter2);
  double discriminant = ((b * b) - (4.0 * (a * c)));

  if (discriminant >= 0) {
    discriminant = std::sqrt(discriminant);
    double t1 = (-b - discriminant) / (2.0 * a);
    double t2 = (-b + discriminant) / (2.0 * a);

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

QCurvature PathFollower::calculateCurvature(const State& istate, const Vector& ilookPoint) {
  MathPoint pos(istate);
  MathPoint look(ilookPoint);
  MathPoint diff = look - pos;
  double heading = ((istate.theta * -1) + 90_deg).convert(radian);
  double a = -std::tan(heading);
  double c = std::tan(heading) * pos.x - pos.y;
  double x = std::abs(a * look.x + 1.0 * look.y + c) / std::sqrt(std::pow(a, 2) + 1);
  int side = sgn(std::sin(heading) * diff.x - std::cos(heading) * diff.y);
  double curv = (2.0 * x) / std::pow(MathPoint::dist(istate, ilookPoint), 2);
  return curvature * curv * side;
}

std::valarray<QSpeed> PathFollower::calculateVelocity(const QSpeed& ivel,
                                                      const QCurvature& icurvature,
                                                      const QLength& ichassisWidth) {
  return {ivel * (2.0 + ichassisWidth.convert(meter) * icurvature.convert(curvature)) / 2.0,
          ivel * (2.0 - ichassisWidth.convert(meter) * icurvature.convert(curvature)) / 2.0};
}

} // namespace lib7842