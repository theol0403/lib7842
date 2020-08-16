#include "lib7842/api/purePursuit/pathFollowerX.hpp"

namespace lib7842 {

using namespace util;

PathFollowerX::PathFollowerX(const std::shared_ptr<XDriveModel>& imodel,
                             const std::shared_ptr<Odometry>& iodometry,
                             const ChassisScales& ichassisScales, const QAngularSpeed& igearset,
                             const QLength& ilookahead) :
  PathFollower(imodel, iodometry, ichassisScales, igearset, ilookahead, ilookahead),
  xModel(imodel) {};

void PathFollowerX::followPath(const std::vector<Waypoint>& path, const PursuitLimits& limits,
                               const std::optional<QSpeed>& startSpeed) {
  resetPursuit();

  auto rate = global::getTimeUtil()->getRate();
  auto timer = global::getTimeUtil()->getTimer();

  // assume the robot starts at minimum velocity unless otherwise specified
  QSpeed lastVelocity = startSpeed.value_or(limits.minVel);

  bool isFinished = false; // loop until the robot is considered to have finished the path
  while (!isFinished) {
    // get the robot position and heading
    State pos = State(odometry->getState(StateMode::CARTESIAN));

    auto closest = findClosest(path, pos); // get an iterator to the closest point
    Vector lookPoint = findLookaheadPoint(path, pos); // get the lookahead

    // the robot is considered finished if it has passed the end
    isFinished = closest >= path.end() - 1;

    // get the velocity from the closest point
    auto targetVel = closest->velocity;

    // add an upwards rate limiter to the robot velocity using the formula vf=vi+at
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    auto dT = timer->getDt();
    // get maximum allowable change in velocity
    QSpeed maxVelocity = lastVelocity + dT * limits.accel;
    // limit the velocity
    if (targetVel > maxVelocity) { targetVel = maxVelocity; }
    lastVelocity = targetVel;

    // calculate robot wheel velocities
    QAngularSpeed wheelVel = (targetVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;
    double power = (wheelVel / gearset).convert(number);

    // calculate target angle at lookahead
    auto start = path[lastLookIndex].theta;
    auto end = path[lastLookIndex + 1].theta;
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
    double turnPower = (turnVel / gearset).convert(number);

    // calculate angle to lookahead
    QAngle angleToLook = pos.angleTo(lookPoint);

    // drive toward the lookahead
    strafeVector(xModel, power, turnPower, angleToLook, mode);

    rate->delayUntil(10_ms);
  }

  model->driveVector(0, 0); // apply velocity braking
}
} // namespace lib7842
