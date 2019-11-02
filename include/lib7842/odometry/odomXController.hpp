#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "odomController.hpp"

namespace lib7842 {

class OdomXController : public OdomController {

public:
  /**
   * OdomXController. Implements chassis movement algorithms for the X drive.
   *
   * @param imodel              The chassis model
   * @param iodometry           The chassis odometry
   * @param idistanceController The distance pid controller
   * @param iturnController     The turning pid controller
   * @param iangleController    The angle pid controller, used to keep distance driving straight
   * @param istrafeController   The strafe pid controller
   * @param isettleRadius       The radius from the target point to give up angle correction
   */
  OdomXController(
    std::shared_ptr<XDriveModel> imodel,
    std::shared_ptr<CustomOdometry> iodometry,
    std::unique_ptr<IterativePosPIDController> idistanceController,
    std::unique_ptr<IterativePosPIDController> iturnController,
    std::unique_ptr<IterativePosPIDController> iangleController,
    std::unique_ptr<IterativePosPIDController> istrafeController,
    const QLength& isettleRadius);

  virtual ~OdomXController() = default;

  // /**
  //  * Drive a distance while correcting angle using an AngleCalculator
  //  *
  //  * @param distance        The distance
  //  * @param angleCalculator The angle calculator
  //  * @param turnScale       The turn scale
  //  * @param settler         The settler
  //  */
  // virtual void moveDistanceAtAngle(
  //   const QLength& distance,
  //   const AngleCalculator& angleCalculator,
  //   double turnScale,
  //   const Settler& settler = defaultStrafeSettler);

  // /**
  //  * Drive a distance while maintaining starting angle
  //  *
  //  * @param distance The distance
  //  * @param settler  The settler
  //  */
  // virtual void moveDistance(const QLength& distance, const Settler& settler = defaultStrafeSettler);

  /**
   * Drive to a point using custom point seeking
   *
   * @param targetPoint The target point
   * @param turnScale   The turn scale used to control the priority of turning over driving. A higher value will make
   *                    the robot turn to face the point sooner
   * @param settler     The settler
   */
  void driveToPoint(
    const Vector& targetPoint,
    double turnScale = 1,
    const Settler& settler = defaultStrafeSettler) override;

  /**
   * A Settler that is used for driving/strafing which uses the distance and strafe pid's isSettled() method
   */
  static bool defaultStrafeSettler(const OdomController& odom);

protected:
  /**
   * Controls the chassis movement. Applies magnitude control to prioritize turning.
   *
   * @param forwardSpeed The forward speed
   * @param yaw          The yaw speed
   * @param strafe       The strafe speed
   */
  void driveXVector(double forwardSpeed, double yaw, double strafe);

  /**
   * Resets the pid controllers, used before every motion
   */
  void resetPid() override;

  std::shared_ptr<XDriveModel> model {nullptr};
  std::unique_ptr<IterativePosPIDController> strafeController {nullptr};
};

} // namespace lib7842
