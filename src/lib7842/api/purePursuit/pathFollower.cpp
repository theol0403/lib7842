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
    // get the lookahead point
    Vector lookPoint = findLookaheadPoint(pos);
    // the robot is on the path if the distance to the closest point is smaller than the lookahead
    bool onPath = Vector::dist(pos, **closest) < lookahead;

    // project the lookahead point onto the lookahead radius. When the lookahead point is further
    // than the lookahead radius, this can cause some problems with the robot curvature calculation.
    // The projected point will cause the robot to rotate more appropriately.
    Vector projectedLookPoint =
      (Vector::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

    // use the normal lookahead point if the robot is on the path and the distance to the lookahead
    // is smaller than the distance to the projected point
    Vector& finalLookPoint =
      onPath && Vector::dist(pos, lookPoint) < Vector::dist(pos, projectedLookPoint)
        ? lookPoint
        : projectedLookPoint;

    // calculate the curvature in order for the robot to arc to the lookahead
    QCurvature curv = calculateCurvature(pos, finalLookPoint);

    // the robot is considered finished if it is on the path, the closest point is the end of the
    // path, and the lookahead is the end of the path
    isFinished = onPath && (closest >= ipath().end() - 1) && lastLookIndex >= ipath().size() - 2;

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

    // calculate robot wheel velocities
    auto robotVel = calculateVelocity(targetVel, curv, odometry->getScales().wheelTrack);

    // convert to rpm
    QAngularSpeed leftWheel =
      (robotVel[0] / (1_pi * odometry->getScales().wheelDiameter)) * 360_deg;
    QAngularSpeed rightWheel =
      (robotVel[1] / (1_pi * odometry->getScales().wheelDiameter)) * 360_deg;

    model->left(leftWheel.convert(rpm));
    model->right(rightWheel.convert(rpm));

    pros::delay(10);
  }

  model->tank(0, 0);
}

PursuitPath::array_t::const_iterator PathFollower::findClosest(const PursuitPath& ipath,
                                                               const Vector& ipos) {
  QLength closestDist {std::numeric_limits<double>::max()};
  auto closest = lastClosest.value_or(ipath().begin());

  // Optimization:
  // limit the progression of the closest point
  // it considers the last closest point, and all the options up to the lookahead + 1
  // if the lookahead is 0, then new options will never be discovered unless the closest searches beyond
  // so it searches one point beyond the lookahead, and if that's closer, it will choose that
  // then later the lookahead will be bumped so it's not behind closest
  // this makes it so the closest can consider pushing the lookahead forward
  // the reason it does not scan all options so that the closest won't catch a much further point in an intersection

  // loop from the last closest point to one point past the lookahead
  for (auto it = closest; it <= ipath().begin() + lastLookIndex + 1; it++) {
    QLength distance = Vector::dist(ipos, **it);
    if (distance < closestDist) {
      closestDist = distance;
      closest = it;
    }
  }

  lastClosest = closest;
  return closest;
}

} // namespace lib7842