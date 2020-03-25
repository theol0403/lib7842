#include "lib7842/api/odometry/odomXController.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

using namespace util;

OdomXController::OdomXController(const std::shared_ptr<XDriveModel>& imodel,
                                 const std::shared_ptr<Odometry>& iodometry,
                                 std::unique_ptr<IterativePosPIDController> idistanceController,
                                 std::unique_ptr<IterativePosPIDController> iturnController,
                                 std::unique_ptr<IterativePosPIDController> iangleController,
                                 const QLength& idriveRadius) :
  OdomController(imodel, iodometry, std::move(idistanceController), std::move(iturnController),
                 std::move(iangleController), idriveRadius),
  xModel(imodel) {};

void OdomXController::strafeRelativeDirection(const QLength& distance, const QAngle& direction,
                                              const Angler& angleCalculator, double turnScale,
                                              Settler&& settler) {
  QAngle absoluteDirection = direction + getState().theta;
  strafeAbsoluteDirection(distance, absoluteDirection, angleCalculator, turnScale,
                          std::move(settler));
}

void OdomXController::strafeAbsoluteDirection(const QLength& distance, const QAngle& direction,
                                              const Angler& angleCalculator, double turnScale,
                                              Settler&& settler) {
  QLength x = sin(direction.convert(radian)) * distance;
  QLength y = cos(direction.convert(radian)) * distance;
  Vector target = Vector(State(getState())) + Vector(x, y);
  strafeToPoint(target, angleCalculator, turnScale, std::move(settler));
}

void OdomXController::strafeToPoint(const Vector& targetPoint, const Angler& angleCalculator,
                                    double turnScale, Settler&& settler) {
  resetPid();
  auto rate = global::getTimeUtil()->getRate();
  do {
    State state = getState();
    _distanceErr = state.distTo(targetPoint);
    _angleErr = angleCalculator(*this);

    QAngle angleToTarget = angleToPoint(targetPoint);

    double distanceVel = distanceController->step(-_distanceErr.convert(millimeter));
    double angleVel = angleController->step(-_angleErr.convert(degree));

    strafeVector(xModel, distanceVel, angleVel * turnScale, angleToTarget);
    rate->delayUntil(10_ms);
  } while (!settler(this));

  driveVector(xModel, 0, 0);
}

} // namespace lib7842
