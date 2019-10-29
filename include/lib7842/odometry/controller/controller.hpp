#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "lib7842/odometry/customOdometry.hpp"

namespace lib7842 {

class OdomController;

using Settler = std::function<bool(OdomController* instance)>;
using Turner = std::function<void(OdomController* instance, double vel)>;
using AngleCalculator = std::function<QAngle(OdomController* instance)>;

using namespace lib7842::OdomMath;

class OdomController {

public:
  OdomController(
    std::shared_ptr<ChassisModel> imodel,
    std::shared_ptr<CustomOdometry> iodometry,
    std::unique_ptr<IterativePosPIDController> idistanceController,
    std::unique_ptr<IterativePosPIDController> iturnController,
    std::unique_ptr<IterativePosPIDController> iangleController);

  virtual ~OdomController() = default;

  void driveVector(double forwardSpeed, double yaw);
  void resetPid();

  QAngle angleToPoint(const Vector& point);
  QLength distanceToPoint(const Vector& point);

  void turn(const AngleCalculator&, const Turner& = pointTurn, const Settler& = turnSettle);
  void turnToAngle(const QAngle&, const Turner& = pointTurn, const Settler& = turnSettle);
  void turnAngle(const QAngle&, const Turner& = pointTurn, const Settler& = turnSettle);
  void turnToPoint(const Vector&, const Turner& = pointTurn, const Settler& = turnSettle);

  void driveDistanceAtAngle(
    const QLength& distance,
    const AngleCalculator& turnCalc,
    double turnScale,
    const Settler& settleFunc = driveSettle);

  void driveDistance(const QLength& distance, const Settler& settleFunc = driveSettle);

  void driveToPoint(const Vector&, double = 1, const Settler& = driveSettle);
  void driveToPoint2(const Vector&, double = 1, const Settler& = driveSettle);

  static Settler makeSettle(const QAngle&);
  static Settler makeSettle(const QLength&);
  static Settler makeSettle(const QLength&, const QAngle&);
  static bool turnSettle(OdomController*);
  static bool driveSettle(OdomController*);

  static void pointTurn(OdomController*, double);
  static void leftPivot(OdomController*, double);
  static void rightPivot(OdomController*, double);

  static AngleCalculator angleCalc(const QAngle&);
  static AngleCalculator angleCalc(const Vector&);
  static AngleCalculator angleCalc();

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<CustomOdometry> odometry {nullptr};
  std::unique_ptr<IterativePosPIDController> distanceController {nullptr};
  std::unique_ptr<IterativePosPIDController> angleController {nullptr};
  std::unique_ptr<IterativePosPIDController> turnController {nullptr};

  const QLength pointRadius; //radius to point before slowing down and ignoring angle

  QAngle angleErr = 0_deg;
  QLength distanceErr = 0_in;
};

} // namespace lib7842
