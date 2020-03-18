#include "pathFollowerX.hpp"

namespace lib7842 {

using namespace util;

PathFollowerX::PathFollowerX(const std::shared_ptr<XDriveModel>& imodel,
                             const std::shared_ptr<Odometry>& iodometry,
                             const ChassisScales& ichassisScales, const QLength& ilookahead) :
  PathFollower(imodel, iodometry, ichassisScales, ilookahead), xModel(imodel) {};

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
    Vector lookPoint = findLookaheadPoint(ipath, pos); // get the lookahead

    // the robot is considered finished if it has passed the end
    isFinished = closest >= ipath().end() - 1;

    // get the velocity from the closest point
    auto targetVel = closest->get()->getData<QSpeed>("velocity");

    // add an upwards rate limiter to the robot velocity using the formula vf=vi+at
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    auto dT = timer->getDt();
    // get maximum allowable change in velocity
    QSpeed maxVelocity = lastVelocity + dT * limits.accel;
    // limit the velocity
    if (targetVel > maxVelocity) targetVel = maxVelocity;
    lastVelocity = targetVel;

    // calculate robot wheel velocities
    QAngularSpeed wheelVel = (targetVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;
    double power = wheelVel.convert(rpm) / 200.0;

    // calculate target angle at lookahead
    QAngle start = ipath()[lastLookIndex]->getData<QAngle>("angle");
    QAngle end = ipath()[lastLookIndex + 1]->getData<QAngle>("angle");
    QAngle angle = start + ((end - start) * lastLookT);

    // get angle error from robot to lookahead
    QAngle error = angle - pos.theta;
    // get distance to lookahead
    QLength dist = Vector::dist(pos, lookPoint);
    // given robot velocity, approximate time to get to lookahead
    QTime time = dist / targetVel;
    // calculate angular velocity to reach the lookahead angle given the time
    QAngularSpeed rotation = error / time;
    // calculate what speed the wheels need to be moving at
    QAngularSpeed turnVel = rotation * chassisScales.wheelTrack / chassisScales.wheelDiameter;

    // get the voltage
    double turnPower = turnVel.convert(rpm) / 200.0;

    // calculate angle to lookahead
    QAngle angleToLook = pos.angleTo(lookPoint);

    // drive toward the lookahead
    strafeVector(xModel, power, turnPower, angleToLook);

    rate->delayUntil(10_ms);
  }

  model->driveVector(0, 0); // apply velocity braking
}
} // namespace lib7842