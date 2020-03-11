#include "pathFollowerX.hpp"

namespace lib7842 {

PathFollowerX::PathFollowerX(const std::shared_ptr<ChassisModel>& imodel,
                             const std::shared_ptr<Odometry>& iodometry,
                             const ChassisScales& ichassisScales, const QLength& ilookahead) :
  PathFollower(imodel, iodometry, ichassisScales, ilookahead) {};

void PathFollowerX::followPath(const PursuitPath& ipath) {
  resetPursuit();

  auto rate = global::getTimeUtil()->getRate();
  auto timer = global::getTimeUtil()->getTimer();

  PursuitLimits limits = ipath.getLimits();
  QSpeed lastVelocity = limits.minVel; // assume the robot starts at minimum velocity

  bool isFinished = false; // loop until the robot is considered to have finished the path
  while (!isFinished) {
    // get the robot position and heading
    State pos = State(odometry->getState(StateMode::CARTESIAN));

    auto closest = findClosest(ipath, pos); // get an iterator to the closest point
    Vector lookPoint = findLookaheadPoint(ipath, pos);

    // the robot is considered finished if it has passed the end
    isFinished = false;

    QSpeed targetVel = closest->get()->getData<QSpeed>("velocity");

    // add an upwards rate limiter to the robot velocity using the formula vf=vi+at
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    auto dT = timer->getDt();
    // get maximum allowable change in velocity
    QSpeed maxVelocity = lastVelocity + dT * limits.accel;
    // limit the velocity
    if (targetVel > maxVelocity) targetVel = maxVelocity;
    lastVelocity = targetVel;

    // calculate robot wheel velocities
    // auto wheelVel = calculateVelocity(targetVel, curvature, chassisScales, limits);

    // double left = wheelVel[0].convert(rpm) / 200.0;
    // double right = wheelVel[1].convert(rpm) / 200.0;

    // // take any speed that is clipped from one side and move it to the other
    // double maxMag = std::abs(left) > std::abs(right) ? left : right;
    // if (std::abs(maxMag) > 1) {
    //   left -= (std::abs(maxMag) - 1) * util::sgn(maxMag);
    //   right -= (std::abs(maxMag) - 1) * util::sgn(maxMag);
    // }

    // if (!ibackwards) {
    //   model->tank(left, right);
    // } else {
    //   model->tank(-left, -right);
    // }

    rate->delayUntil(10_ms);
  }

  model->driveVector(0, 0); // apply velocity braking
}
} // namespace lib7842
