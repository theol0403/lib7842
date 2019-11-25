#include "pathFollower.hpp"

namespace lib7842 {

PathFollower::PathFollower(const std::shared_ptr<ChassisModel>& imodel,
                           const std::shared_ptr<Odometry>& iodometry,
                           const QLength& ilookahead,
                           const QSpeed& iminVel,
                           const QSpeed& imaxVel,
                           const QAcceleration& iaccel,
                           double ik) :
  model(imodel),
  odometry(iodometry),
  lookahead(ilookahead),
  minVel(iminVel),
  maxVel(imaxVel),
  accel(iaccel),
  k(ik) {}

void PathFollower::followPath(const DataPath& ipath) {

  // get the starting position
  State lastPos = State(odometry->getState(StateMode::CARTESIAN));
  // assume the robot starts at minimum velocity
  QSpeed lastVelocity = minVel;

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
                                 k / std::abs(curv.convert(curvature)));
    } else {
      targetVel = mps * std::min(maxVel.convert(mps), k / std::abs(curv.convert(curvature)));
    }

    // add an upwards rate limiter to the robot velocity. Assume the robot starts at the minimum
    // velocity and prevent the robot from going slower than it. Calculate the distance travelled
    // since the last calculation and calculate maximum change in velocity acording to acceleration.
    targetVel = std::max(targetVel, minVel); // add minimum velocity
    // get distance traveled since last calculation
    QLength distDt = Vector::dist(lastPos, pos);
    // get maximum allowable change in velocity
    QSpeed maxVelocity = mps * std::sqrt(std::pow(lastVelocity.convert(mps), 2) +
                                         (2 * accel.convert(mps2) * distDt.convert(meter)));
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

    model->tank(leftWheel.convert(rpm), rightWheel.convert(rpm));

    pros::delay(10);
  }
}

} // namespace lib7842