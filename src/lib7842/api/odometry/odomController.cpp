#include "odomController.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

using namespace util;

OdomController::OdomController(const std::shared_ptr<ChassisModel>& imodel,
                               const std::shared_ptr<Odometry>& iodometry,
                               std::unique_ptr<IterativePosPIDController> idistanceController,
                               std::unique_ptr<IterativePosPIDController> iturnController,
                               std::unique_ptr<IterativePosPIDController> iangleController,
                               const QLength& isettleRadius, const TimeUtil& itimeUtil) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  distanceController(std::move(idistanceController)),
  angleController(std::move(iangleController)),
  turnController(std::move(iturnController)),
  settleRadius(isettleRadius),
  timeUtil(itimeUtil) {};

/**
 * Turning API
 */
void OdomController::turn(const AngleCalculator& angleCalculator, const Turner& turner,
                          const Settler& settler) {
  resetPid();
  auto rate = timeUtil.getRate();
  do {
    angleErr = angleCalculator(*this);
    double vel = turnController->step(-angleErr.convert(degree));
    turner(*model, vel);
    rate->delayUntil(10_ms);
  } while (!settler(*this));
  turner(*model, 0);
}

void OdomController::turnToAngle(const QAngle& angle, const Turner& turner,
                                 const Settler& settler) {
  turn(makeAngleCalculator(angle), turner, settler);
}

void OdomController::turnAngle(const QAngle& angle, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(angle + getState().theta), turner, settler);
}

void OdomController::turnToPoint(const Vector& point, const Turner& turner,
                                 const Settler& settler) {
  turn(makeAngleCalculator(point), turner, settler);
}

/**
 * Driving API
 */
void OdomController::moveDistanceAtAngle(const QLength& distance,
                                         const AngleCalculator& angleCalculator, double turnScale,
                                         const Settler& settler) {
  resetPid();
  auto rate = timeUtil.getRate();
  auto lastTicks = model->getSensorVals();
  do {
    auto newTicks = model->getSensorVals();
    QLength leftDistance = ((newTicks[0] - lastTicks[0]) / odometry->getScales().straight) * meter;
    QLength rightDistance = ((newTicks[1] - lastTicks[1]) / odometry->getScales().straight) * meter;

    distanceErr = distance - ((leftDistance + rightDistance) / 2);
    angleErr = angleCalculator(*this);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    driveVector(model, distanceVel, angleVel * turnScale);
    rate->delayUntil(10_ms);
  } while (!settler(*this));

  driveVector(model, 0, 0);
}

void OdomController::moveDistance(const QLength& distance, const Settler& settler) {
  moveDistanceAtAngle(distance, makeAngleCalculator(getState().theta), 1, settler);
}

/**
 * Point API
 */
void OdomController::driveToPoint(const Vector& targetPoint, double turnScale,
                                  const Settler& settler) {
  resetPid();
  auto rate = timeUtil.getRate();
  do {
    State state = getState();
    Vector closestPoint = closest(state, targetPoint);

    QAngle angleToClose = state.angleTo(closestPoint);
    QAngle angleToTarget = state.angleTo(targetPoint);

    QLength distanceToClose = state.distTo(closestPoint);
    QLength distanceToTarget = state.distTo(targetPoint);

    // go backwards
    if (angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

    if (distanceToTarget.abs() < settleRadius) {
      angleErr = 0_deg;
      // used for settling
      distanceErr = distanceToClose;
    } else {
      angleErr = angleToTarget;
      // used for settling
      distanceErr = distanceToTarget;
    }

    // rotate angle to be +- 90
    angleErr = rotateAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceToClose.convert(millimeter));

    driveVector(model, distanceVel, angleVel * turnScale);
    rate->delayUntil(10_ms);
  } while (!settler(*this));

  driveVector(model, 0, 0);
}

void OdomController::driveToPoint2(const Vector& targetPoint, double turnScale,
                                   const Settler& settler) {
  resetPid();
  auto rate = timeUtil.getRate();
  Settler exitFunc = makeSettler(settleRadius);
  do {
    State state = getState();
    angleErr = state.angleTo(targetPoint);
    distanceErr = state.distTo(targetPoint);

    if (angleErr.abs() > 90_deg) distanceErr = -distanceErr;
    angleErr = rotateAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));

    driveVector(model, distanceVel, angleVel * turnScale);
    rate->delayUntil(10_ms);
  } while (!(exitFunc(*this) || settler(*this)));

  moveDistanceAtAngle(distanceToPoint(targetPoint), makeAngleCalculator(angleToPoint(targetPoint)),
                      turnScale, settler);
  driveVector(model, 0, 0);
}

/**
 * Default Settlers
 */
bool OdomController::defaultTurnSettler(const OdomController& odom) {
  return odom.turnController->isSettled();
}

bool OdomController::defaultDriveSettler(const OdomController& odom) {
  return odom.distanceController->isSettled();
}

bool OdomController::defaultDriveAngleSettler(const OdomController& odom) {
  return odom.distanceController->isSettled() && odom.angleController->isSettled();
}

/**
 * Default Turners
 */
void OdomController::pointTurn(ChassisModel& model, double vel) {
  model.tank(vel, -vel);
}

void OdomController::leftPivot(ChassisModel& model, double vel) {
  model.tank(vel * 2, 0);
}

void OdomController::rightPivot(ChassisModel& model, double vel) {
  model.tank(0, -vel * 2);
}

/**
 * Settler Generators
 */
Settler OdomController::makeSettler(const QAngle& angle) {
  return [=](const OdomController& odom) {
    return odom.angleErr.abs() < angle;
  };
}

Settler OdomController::makeSettler(const QLength& distance) {
  return [=](const OdomController& odom) {
    return odom.distanceErr.abs() < distance;
  };
}

Settler OdomController::makeSettler(const QLength& distance, const QAngle& angle) {
  return [=](const OdomController& odom) {
    return odom.distanceErr.abs() < distance && odom.angleErr.abs() < angle;
  };
}

/**
 * AngleCalculator Generators
 */
AngleCalculator OdomController::makeAngleCalculator(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](const OdomController& odom) {
    return rollAngle180(iangle - odom.getState().theta);
  };
}

AngleCalculator OdomController::makeAngleCalculator(const Vector& point) {
  return [=](const OdomController& odom) {
    return odom.angleToPoint(point);
  };
}

AngleCalculator OdomController::makeAngleCalculator(double error) {
  return [=](const OdomController&) {
    return error * degree;
  };
}

AngleCalculator OdomController::makeAngleCalculator() {
  return [=](const OdomController&) {
    return 0_deg;
  };
}

/**
 * OdomController utilities
 */
State OdomController::getState() const {
  return State(odometry->getState(StateMode::CARTESIAN));
}

QLength OdomController::distanceToPoint(const Vector& point) const {
  return getState().distTo(point);
}

QAngle OdomController::angleToPoint(const Vector& point) const {
  return getState().angleTo(point);
}

void OdomController::resetPid() {
  distanceController->reset();
  turnController->reset();
  angleController->reset();
  distanceErr = 0_in;
  angleErr = 0_deg;
}

} // namespace lib7842
