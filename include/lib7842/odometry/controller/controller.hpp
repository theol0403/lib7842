#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "lib7842/odometry/customOdometry.hpp"

namespace lib7842 {

class OdomController;

using settleFunc_t = std::function<bool(OdomController* instance)>;
using turnFunc_t = std::function<void(OdomController* instance, double vel)>;
using angleCalc_t = std::function<QAngle(OdomController* instance)>;

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

  void turn(const angleCalc_t&, const turnFunc_t& = pointTurn, const settleFunc_t& = turnSettle);
  void turnToAngle(const QAngle&, const turnFunc_t& = pointTurn, const settleFunc_t& = turnSettle);
  void turnAngle(const QAngle&, const turnFunc_t& = pointTurn, const settleFunc_t& = turnSettle);
  void turnToPoint(const Vector&, const turnFunc_t& = pointTurn, const settleFunc_t& = turnSettle);

  void driveDistanceAtAngle(
    const QLength& distance,
    const angleCalc_t& turnCalc,
    double turnScale,
    const settleFunc_t& settleFunc = driveSettle);

  void driveDistance(const QLength& distance, const settleFunc_t& settleFunc = driveSettle);

  void driveToPoint(const Vector&, double = 1, const settleFunc_t& = driveSettle);
  void driveToPoint2(const Vector&, double = 1, const settleFunc_t& = driveSettle);

  static settleFunc_t makeSettle(const QAngle&);
  static settleFunc_t makeSettle(const QLength&);
  static settleFunc_t makeSettle(const QLength&, const QAngle&);
  static bool turnSettle(OdomController*);
  static bool driveSettle(OdomController*);

  static void pointTurn(OdomController*, double);
  static void leftPivot(OdomController*, double);
  static void rightPivot(OdomController*, double);

  static angleCalc_t angleCalc(const QAngle&);
  static angleCalc_t angleCalc(const Vector&);
  static angleCalc_t angleCalc();

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
