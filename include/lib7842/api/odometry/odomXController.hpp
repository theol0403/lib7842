#pragma once

#include "odomController.hpp"
#include "odomMath.hpp"
#include "okapi/api/chassis/model/xDriveModel.hpp"

namespace lib7842 {

/**
 * Odometry motion controller for X-Base chassis.
 */
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
   */
  OdomXController(const std::shared_ptr<XDriveModel>& imodel,
                  const std::shared_ptr<Odometry>& iodometry,
                  std::unique_ptr<IterativePosPIDController> idistanceController,
                  std::unique_ptr<IterativePosPIDController> iturnController,
                  std::unique_ptr<IterativePosPIDController> iangleController);

  virtual ~OdomXController() = default;

  /**
   * Strafe a distance in a relative direction while correcting angle using an AngleCalculator
   *
   * @param distance        The distance
   * @param direction       The relative direction of the strafing
   * @param angleCalculator The angle calculator
   * @param turnScale       The turn scale
   * @param settler         The settler
   */
  virtual void
    strafeRelativeDirection(const QLength& distance,
                            const QAngle& direction,
                            const AngleCalculator& angleCalculator = makeAngleCalculator(),
                            double turnScale = 1,
                            const Settler& settler = defaultDriveSettler);

  /**
   * Strafe a distance in an absolute direction while correcting angle using an AngleCalculator
   *
   * @param distance        The distance
   * @param direction       The absolute direction of the strafing
   * @param angleCalculator The angle calculator
   * @param turnScale       The turn scale
   * @param settler         The settler
   */
  virtual void
    strafeAbsoluteDirection(const QLength& distance,
                            const QAngle& direction,
                            const AngleCalculator& angleCalculator = makeAngleCalculator(),
                            double turnScale = 1,
                            const Settler& settler = defaultDriveSettler);

  /**
   * Strafe to a point using field-centric math and an AngleCalculator
   *
   * @param targetPoint     The target point
   * @param angleCalculator The angle calculator
   * @param turnScale       The turn scale
   * @param settler         The settler
   */
  virtual void strafeToPoint(const Vector& targetPoint,
                             const AngleCalculator& angleCalculator = makeAngleCalculator(),
                             double turnScale = 1,
                             const Settler& settler = defaultDriveSettler);

protected:
  /**
   * Control the chassis movement. Applies magnitude control to prioritize turning.
   *
   * @param forwardSpeed The forward speed
   * @param yaw          The yaw speed
   * @param strafe       The strafe speed
   */
  void driveXVector(double forwardSpeed, double yaw, double strafe);

  /**
   * Control the chassis movement. Strafes at the given speed at the given direction.
   *
   * @param speed     The speed
   * @param direction The direction
   * @param yaw       The yaw
   */
  void strafeXVector(double speed, const QAngle& direction, double yaw);

  std::shared_ptr<XDriveModel> xModel {nullptr};
};

} // namespace lib7842
