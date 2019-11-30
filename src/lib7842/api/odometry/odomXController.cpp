#include "odomXController.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;
using namespace util;

OdomXController::OdomXController(const std::shared_ptr<XDriveModel>& imodel,
                                 const std::shared_ptr<Odometry>& iodometry,
                                 std::unique_ptr<IterativePosPIDController> idistanceController,
                                 std::unique_ptr<IterativePosPIDController> iturnController,
                                 std::unique_ptr<IterativePosPIDController> iangleController) :
  OdomController(imodel,
                 iodometry,
                 std::move(idistanceController),
                 std::move(iturnController),
                 std::move(iangleController),
                 0_in),
  xModel(imodel) {};

void OdomXController::strafeRelativeDirection(const QLength& distance,
                                              const QAngle& direction,
                                              const AngleCalculator& angleCalculator,
                                              double turnScale,
                                              const Settler& settler) {
  State state = State(odometry->getState(StateMode::CARTESIAN));
  QAngle absoluteDirection = direction + state.theta;
  QLength x = sin(absoluteDirection.convert(radian)) * distance;
  QLength y = cos(absoluteDirection.convert(radian)) * distance;
  Vector target = Vector(state) + Vector(x, y);
  strafeToPoint(target, angleCalculator, turnScale, settler);
}

void OdomXController::strafeAbsoluteDirection(const QLength& distance,
                                              const QAngle& direction,
                                              const AngleCalculator& angleCalculator,
                                              double turnScale,
                                              const Settler& settler) {
  QLength x = sin(direction.convert(radian)) * distance;
  QLength y = cos(direction.convert(radian)) * distance;
  Vector target = Vector(State(odometry->getState(StateMode::CARTESIAN))) + Vector(x, y);
  strafeToPoint(target, angleCalculator, turnScale, settler);
}

void OdomXController::strafeToPoint(const Vector& targetPoint,
                                    const AngleCalculator& angleCalculator,
                                    double turnScale,
                                    const Settler& settler) {
  resetPid();
  do {
    distanceErr = distanceToPoint(targetPoint);
    angleErr = angleCalculator(*this);

    QAngle angleToTarget = angleToPoint(targetPoint);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    strafeVector(xModel, distanceVel, angleVel * turnScale, angleToTarget);
    pros::delay(10);
  } while (!settler(*this));

  driveVector(xModel, 0, 0);
}

} // namespace lib7842
