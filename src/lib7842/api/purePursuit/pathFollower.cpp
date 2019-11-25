#include "pathFollower.hpp"

namespace lib7842 {

PathFollower::PathFollower(const std::shared_ptr<ChassisModel>& imodel,
                           const std::shared_ptr<Odometry>& iodometry,
                           const QLength& ilookahead) :
  model(imodel), odometry(iodometry), lookahead(ilookahead) {}

void PathFollower::followPath(const DataPath& ipath) {
  State pos = State(odometry->getState(StateMode::CARTESIAN));

  auto closest = findClosest(ipath, pos);
  bool onPath = Vector::dist(pos, **closest) < lookahead;

  Vector lookPoint = findLookaheadPoint(pos);

  Vector projectedLookPoint = (Vector::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

  Vector& finalLookPoint =
    onPath && Vector::dist(pos, lookPoint) < Vector::dist(pos, projectedLookPoint)
      ? lookPoint
      : projectedLookPoint;

  QCurvature curvature = calculateCurvature(pos, finalLookPoint);
}

} // namespace lib7842