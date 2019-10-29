#include "controller.hpp"

namespace lib7842 {

void OdomController::pointTurn(OdomController* that, double turnVel) {
  that->model->rotate(turnVel);
}

void OdomController::leftPivot(OdomController* that, double turnVel) {
  that->model->left(turnVel * 2);
}

void OdomController::rightPivot(OdomController* that, double turnVel) {
  that->model->right(-turnVel * 2);
}

AngleCalculator OdomController::angleCalc(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](OdomController* that) {
    return rollAngle180(iangle - that->odometry->getState().theta);
  };
}

AngleCalculator OdomController::angleCalc(const Vector& point) {
  return [=](OdomController* that) {
    return that->angleToPoint(point);
  };
}

AngleCalculator OdomController::angleCalc() {
  return [=](OdomController*) {
    return 0_deg;
  };
}

void OdomController::turn(
  const AngleCalculator& turnCalc, const Turner& turnFunc, const Settler& settleFunc) {
  resetPid();
  do {
    angleErr = turnCalc(this);
    double turnVel = turnController->step(-angleErr.convert(degree));
    turnFunc(this, turnVel);
    pros::delay(10);
  } while (!settleFunc(this));
  turnFunc(this, 0);
}

void OdomController::turnToAngle(
  const QAngle& angle, const Turner& turnFunc, const Settler& settleFunc) {
  turn(angleCalc(angle), turnFunc, settleFunc);
}

void OdomController::turnAngle(
  const QAngle& angle, const Turner& turnFunc, const Settler& settleFunc) {
  turn(angleCalc(angle + odometry->getState().theta), turnFunc, settleFunc);
}

void OdomController::turnToPoint(
  const Vector& point, const Turner& turnFunc, const Settler& settleFunc) {
  turn(angleCalc(point), turnFunc, settleFunc);
}

} // namespace lib7842
