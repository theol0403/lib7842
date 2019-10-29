#include "controller.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

void OdomController::pointTurn(OdomController* instance, double turnVel) {
  instance->model->rotate(turnVel);
}

void OdomController::leftPivot(OdomController* instance, double turnVel) {
  instance->model->left(turnVel * 2);
}

void OdomController::rightPivot(OdomController* instance, double turnVel) {
  instance->model->right(-turnVel * 2);
}

AngleCalculator OdomController::makeAngleCalculator(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](OdomController* instance) {
    return rollAngle180(iangle - instance->odometry->getState().theta);
  };
}

AngleCalculator OdomController::makeAngleCalculator(const Vector& point) {
  return [=](OdomController* instance) {
    return instance->angleToPoint(point);
  };
}

AngleCalculator OdomController::makeAngleCalculator() {
  return [=](OdomController*) {
    return 0_deg;
  };
}

void OdomController::turn(
  const AngleCalculator& angleCalculator, const Turner& turner, const Settler& settler) {
  resetPid();
  do {
    angleErr = angleCalculator(this);
    double turnVel = turnController->step(-angleErr.convert(degree));
    turner(this, turnVel);
    pros::delay(10);
  } while (!settler(this));
  turner(this, 0);
}

void OdomController::turnToAngle(
  const QAngle& angle, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(angle), turner, settler);
}

void OdomController::turnAngle(const QAngle& angle, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(angle + odometry->getState().theta), turner, settler);
}

void OdomController::turnToPoint(
  const Vector& point, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(point), turner, settler);
}

} // namespace lib7842
