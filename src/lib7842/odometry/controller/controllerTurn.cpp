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

angleCalc_t OdomController::angleCalc(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](OdomController* that) {
    return rollAngle180(iangle - that->odometry->getState().theta);
  };
}

angleCalc_t OdomController::angleCalc(const Vector& point) {
  return [=](OdomController* that) {
    return that->angleToPoint(point);
  };
}

angleCalc_t OdomController::angleCalc() {
  return [=](OdomController*) {
    return 0_deg;
  };
}

void OdomController::turn(
  const angleCalc_t& turnCalc, const turnFunc_t& turnFunc, const settleFunc_t& settleFunc) {
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
  const QAngle& angle, const turnFunc_t& turnFunc, const settleFunc_t& settleFunc) {
  turn(angleCalc(angle), turnFunc, settleFunc);
}

void OdomController::turnAngle(
  const QAngle& angle, const turnFunc_t& turnFunc, const settleFunc_t& settleFunc) {
  turn(angleCalc(angle + odometry->getState().theta), turnFunc, settleFunc);
}

void OdomController::turnToPoint(
  const Vector& point, const turnFunc_t& turnFunc, const settleFunc_t& settleFunc) {
  turn(angleCalc(point), turnFunc, settleFunc);
}

} // namespace lib7842
