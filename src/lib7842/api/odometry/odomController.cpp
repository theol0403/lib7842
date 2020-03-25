#include "lib7842/api/odometry/odomController.hpp"

#include "pros/rtos.hpp"
#include <utility>

namespace lib7842 {

using namespace util;

OdomController::OdomController(std::shared_ptr<ChassisModel> imodel,
                               std::shared_ptr<Odometry> iodometry,
                               std::unique_ptr<IterativePosPIDController> idistanceController,
                               std::unique_ptr<IterativePosPIDController> iturnController,
                               std::unique_ptr<IterativePosPIDController> iangleController,
                               const QLength& idriveRadius) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  distanceController(std::move(idistanceController)),
  angleController(std::move(iangleController)),
  turnController(std::move(iturnController)),
  driveRadius(idriveRadius) {};

OdomController::Angler OdomController::makeAngler() {
  return [=](const OdomController& /*unused*/) {
    return 0_deg;
  };
}

OdomController::Angler OdomController::makeAngler(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](const OdomController& odom) {
    return rollAngle180(iangle - odom.getState().theta);
  };
}

OdomController::Angler OdomController::makeAngler(const Vector& point) {
  return [=](const OdomController& odom) {
    return odom.angleToPoint(point);
  };
}

OdomController::Turner OdomController::pointTurn = [](const std::shared_ptr<ChassisModel>& imodel,
                                                      double vel) {
  imodel->tank(vel, -vel);
};

OdomController::Turner OdomController::leftPivot = [](const std::shared_ptr<ChassisModel>& imodel,
                                                      double vel) {
  imodel->tank(vel * 2, 0);
};

OdomController::Turner OdomController::rightPivot = [](const std::shared_ptr<ChassisModel>& imodel,
                                                       double vel) {
  imodel->tank(0, -vel * 2);
};

void OdomController::turn(const Angler& angler, const Turner& turner, Settler&& settler) {
  settler
    .noAbort(); // this algorithm does not emit distance error, so we don't want a false positive
  resetPid();
  auto rate = global::getTimeUtil()->getRate();
  do {
    _angleErr = angler(*this);
    double vel = turnController->step(-_angleErr.convert(degree));
    turner(model, vel);
    rate->delayUntil(10_ms);
  } while (!settler(this));
  turner(model, 0);
}

void OdomController::turnToAngle(const QAngle& angle, const Turner& turner, Settler&& settler) {
  turn(makeAngler(angle), turner, std::move(settler));
}

void OdomController::turnAngle(const QAngle& angle, const Turner& turner, Settler&& settler) {
  turn(makeAngler(angle + getState().theta), turner, std::move(settler));
}

void OdomController::turnToPoint(const Vector& point, const Turner& turner, Settler&& settler) {
  turn(makeAngler(point), turner, std::move(settler));
}

void OdomController::moveDistanceAtAngle(const QLength& distance, const Angler& angler,
                                         Settler&& settler) {
  resetPid();
  auto rate = global::getTimeUtil()->getRate();
  auto lastTicks = model->getSensorVals();
  do {
    auto newTicks = model->getSensorVals();
    QLength leftDistance = ((newTicks[0] - lastTicks[0]) / odometry->getScales().straight) * meter;
    QLength rightDistance = ((newTicks[1] - lastTicks[1]) / odometry->getScales().straight) * meter;

    _distanceErr = distance - ((leftDistance + rightDistance) / 2);
    _angleErr = angler(*this);

    double distanceVel = distanceController->step(-_distanceErr.convert(millimeter));
    double angleVel = angleController->step(-_angleErr.convert(degree));

    driveVector(model, distanceVel, angleVel);
    rate->delayUntil(10_ms);
  } while (!settler(this));

  driveVector(model, 0, 0);
}

void OdomController::moveDistance(const QLength& distance, Settler&& settler) {
  moveDistanceAtAngle(distance, makeAngler(getState().theta), std::move(settler));
}

void OdomController::driveToPoint(const Vector& point, double turnScale, Settler&& settler) {
  resetPid();
  auto rate = global::getTimeUtil()->getRate();
  do {
    State state = getState();
    Vector closestPoint = closest(state, point);

    QAngle angleToClose = state.angleTo(closestPoint);
    QAngle angleToTarget = state.angleTo(point);

    QLength distanceToClose = state.distTo(closestPoint);
    QLength distanceToTarget = state.distTo(point);

    // go backwards
    if (angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

    if (distanceToTarget.abs() < driveRadius) {
      _angleErr = 0_deg;
      // used for settling
      _distanceErr = distanceToClose;
    } else {
      _angleErr = angleToTarget;
      // used for settling
      _distanceErr = distanceToTarget;
    }

    // rotate angle to be +- 90
    _angleErr = wrapAngle90(_angleErr);

    double angleVel = angleController->step(-_angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceToClose.convert(millimeter));

    driveVector(model, distanceVel, angleVel * turnScale);
    rate->delayUntil(10_ms);
  } while (!settler(this));

  driveVector(model, 0, 0);
}

State OdomController::getState() const {
  return State(odometry->getState(StateMode::CARTESIAN));
}

QLength OdomController::distanceToPoint(const Vector& point) const {
  return getState().distTo(point);
}

QAngle OdomController::angleToPoint(const Vector& point) const {
  return getState().angleTo(point);
}

QLength OdomController::getDistanceError() const {
  return _distanceErr;
}

QAngle OdomController::getAngleError() const {
  return _angleErr;
}

bool OdomController::isDistanceSettled() const {
  return distanceController->isSettled();
}

bool OdomController::isAngleSettled() const {
  return angleController->isSettled();
}

bool OdomController::isTurnSettled() const {
  return turnController->isSettled();
}

Trigger::Function OdomController::distanceTo(const Vector& point, const QLength& trigger) const {
  return [=] {
    return distanceToPoint(point) < trigger;
  };
}

Trigger::Function OdomController::angleTo(const Vector& point, const QAngle& trigger) const {
  return [=] {
    return angleToPoint(point) < trigger;
  };
}

Trigger::Function OdomController::angleTo(const QAngle& angle, const QAngle& trigger) const {
  return [=] {
    return (getState().theta - angle).abs() < trigger;
  };
}

Trigger::Function OdomController::distanceErr(const QLength& trigger) const {
  return [=] {
    return getDistanceError() < trigger;
  };
}

Trigger::Function OdomController::angleErr(const QAngle& trigger) const {
  return [=] {
    return getAngleError() < trigger;
  };
}

Trigger::Function OdomController::distanceSettled() const {
  return [=] {
    return isDistanceSettled();
  };
}

Trigger::Function OdomController::turnSettled() const {
  return [=] {
    return isTurnSettled();
  };
}

Trigger::Function OdomController::angleSettled() const {
  return [=] {
    return isAngleSettled();
  };
}

Trigger::Function OdomController::distanceSettledUtil(const TimeUtil& timeUtil) const {
  return [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
    return settledUtil->isSettled(getDistanceError().convert(millimeter));
  };
}

Trigger::Function OdomController::angleSettledUtil(const TimeUtil& timeUtil) const {
  return [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
    return settledUtil->isSettled(getAngleError().convert(degree));
  };
}

void OdomController::resetPid() {
  distanceController->reset();
  turnController->reset();
  angleController->reset();
  _distanceErr = 0_in;
  _angleErr = 0_deg;
}

} // namespace lib7842
