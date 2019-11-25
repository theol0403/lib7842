#include "pathFollower.hpp"

namespace lib7842 {

PathFollower::PathFollower(const std::shared_ptr<ChassisModel>& imodel,
                           const std::shared_ptr<Odometry>& iodometry,
                           const QLength& ilookahead,
                           const QSpeed& iminVel) :
  model(imodel), odometry(iodometry), lookahead(ilookahead), minVel(iminVel) {}

void PathFollower::followPath(const DataPath& ipath) {

  // get the starting position
  State lastPos = State(odometry->getState(StateMode::CARTESIAN));
  // assume the robot starts at minimum velocity
  QSpeed lastVelocity = minVel;

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

    // project the lookahead point onto the lookahead radius. When the lookahead point is further than
    // the lookahead radius, this can cause some problems with the robot curvature calculation. The
    // projected point will cause the robot to rotate more appropriately.
    Vector projectedLookPoint =
      (Vector::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

    // use the normal lookahead point if the robot is on the path and the distance to the lookahead
    // is smaller than the distance to the projected point
    Vector& finalLookPoint =
      onPath && Vector::dist(pos, lookPoint) < Vector::dist(pos, projectedLookPoint)
        ? lookPoint
        : projectedLookPoint;

    // calculate the curvature for the robot to arc to the lookahead
    QCurvature curvature = calculateCurvature(pos, finalLookPoint);

    // the robot is considered finished if it is on the path, the closest point is the end of the
    // path, and the lookahead is the end of the path
    isFinished = onPath && (closest >= ipath().end() - 1) && lastLookIndex >= ipath().size() - 2;

    pros::delay(10);
  }
}

} // namespace lib7842