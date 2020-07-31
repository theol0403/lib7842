#pragma once
#include "odomController.hpp"
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
   * @param imodel              The chassis model.
   * @param iodometry           The chassis odometry.
   * @param idistanceController The distance PID controller.
   * @param iangleController    The angle PID controller, used to keep distance driving straight.
   * @param iturnController     The turning PID controller, used for turn commands.
   * @param idriveRadius        The radius from the target point to turn off angle correction when
   *                            driving to a point. Used by OdomController's driveToPoint, which is
   *                            not used with an X drive.
   */
  OdomXController(const std::shared_ptr<XDriveModel>& imodel,
                  const std::shared_ptr<Odometry>& iodometry,
                  std::unique_ptr<IterativePosPIDController> idistanceController,
                  std::unique_ptr<IterativePosPIDController> iangleController,
                  std::unique_ptr<IterativePosPIDController> iturnController,
                  const QLength& idriveRadius);

  /**
   * Strafe a distance in a relative direction while correcting angle using an Angler. This method
   * strafes directly to the point, and the Angler can control the heading of the robot along the
   * way. This uses the distance and angle PID controller.
   *
   * @param distance  The linear distance to strafe.
   * @param direction The relative direction to strafe
   * @param angler    The angler that directs the heading of the robot.
   * @param turnScale The turn scale is used to control the priority of turning over driving. A
   *                  higher value will make the robot turn to face the point sooner, and a lower
   *                  value will take longer.
   * @param settler   The settler that tells the drive to stop.
   */
  virtual void
    strafeRelativeDirection(const QLength& distance, const QAngle& direction,
                            const Angler& angler = makeAngler(), double turnScale = 1,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Strafe a distance in an absolute direction while correcting angle using an Angler. This method
   * strafes directly to the point, and the Angler can control the heading of the robot along the
   * way. This uses the distance and angle PID controller.
   *
   * @param distance  The linear distance to strafe.
   * @param direction The absolute direction to strafe.
   * @param angler    The angler that directs the heading of the robot.
   * @param turnScale The turn scale is used to control the priority of turning over driving. A
   *                  higher value will make the robot turn to face the point sooner, and a lower
   *                  value will take longer.
   * @param settler   The settler that tells the drive to stop.
   */
  virtual void
    strafeAbsoluteDirection(const QLength& distance, const QAngle& direction,
                            const Angler& angler = makeAngler(), double turnScale = 1,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Strafe to a point using field-centric math and an Angler. This method strafes directly to the
   * point, and the Angler can control the heading of the robot along the way. This uses the
   * distance and angle PID controller.
   *
   * @param point     The target point to strafe to.
   * @param angler    The angler that directs the heading of the robot.
   * @param turnScale The turn scale is used to control the priority of turning over driving. A
   *                  higher value will make the robot turn to face the point sooner, and a lower
   *                  value will take longer.
   * @param settler   The settler that tells the drive to stop.
   */
  virtual void strafeToPoint(const Vector& point, const Angler& angler = makeAngler(),
                             double turnScale = 1,
                             Settler&& settler = Settler().distanceSettled().angleSettled());

protected:
  std::shared_ptr<XDriveModel> xModel {nullptr};
};
} // namespace lib7842
