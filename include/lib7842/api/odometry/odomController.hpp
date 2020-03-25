#pragma once
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/other/utility.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "settler.hpp"
#include <functional>

namespace lib7842 {
using namespace okapi;
class OdomController;

/**
 * Odometry motion controller for skid-steer chassis.
 */
class OdomController {
public:
  /**
   * OdomController. Implements chassis movement algorithms.
   *
   * @param imodel              The chassis model.
   * @param iodometry           The chassis odometry.
   * @param idistanceController The distance PID controller.
   * @param iturnController     The turning PID controller, used for turn commands.
   * @param iangleController    The angle PID controller, used to keep distance driving straight.
   * @param idriveRadius        The radius from the target point to turn off angle correction when
   *                            driving to a point.
   */
  OdomController(std::shared_ptr<ChassisModel> imodel, std::shared_ptr<Odometry> iodometry,
                 std::unique_ptr<IterativePosPIDController> idistanceController,
                 std::unique_ptr<IterativePosPIDController> iturnController,
                 std::unique_ptr<IterativePosPIDController> iangleController,
                 const QLength& idriveRadius);

  virtual ~OdomController() = default;

  /**
   * An Angler is a function that returns an angle for the chassis to seek using PID.
   */
  using Angler = std::function<QAngle(const OdomController& odom)>;

  /**
   * Make an Angler that returns 0 and does nothing. This is the default Angler.
   *
   * @return An Angler that does nothing.
   */
  static Angler makeAngler();

  /**
   * Make an Angler that seeks an absolute angle. Wraps the angle to take the shortest direction.
   *
   * @param  angle The angle to seek.
   * @return The produced Angler.
   */
  static Angler makeAngler(const QAngle& angle);

  /**
   * Make an Angler that seeks a point. Wraps the angle to take the shortest direction.
   *
   * @param  point The point to seek.
   * @return The produced Angler.
   */
  static Angler makeAngler(const Vector& point);

  /**
   * A Turner is a function that accepts a chassis and a turning velocity. It controls the motor
   * distribution and execution of the turn, and is used to implement a point or pivot turn.
   */
  using Turner = std::function<void(const std::shared_ptr<ChassisModel>& imodel, double vel)>;

  /**
   * The built-in default Turners.
   *  - A Turner that executes a point turn which turns in place using both motors. Used as the
   *    default for all turn functions.
   *  - A Turner that executes a left pivot, meaning it only moves the left motors.
   *  - A Turner that executes a right pivot, meaning it only moves the right motors.
   */
  static Turner pointTurn, leftPivot, rightPivot;

  /**
   * Turn the chassis using an Angler. Uses the the turn PID controller.
   *
   * @param angler  The angler that directs the turn.
   * @param turner  The turner that executes the turn.
   * @param settler The settler that tells the turn to stop.
   */
  virtual void turn(const Angler& angler, const Turner& turner = pointTurn,
                    Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face an absolute angle.
   *
   * @param angle   The absolute angle to seek.
   * @param turner  The turner that executes the turn.
   * @param settler The settler that tells the turn to stop.
   */
  virtual void turnToAngle(const QAngle& angle, const Turner& turner = pointTurn,
                           Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face a relative angle to the current angle.
   *
   * @param angle   The relative angle to seek.
   * @param turner  The turner that executes the turn.
   * @param settler The settler that tells the turn to stop.
   */
  virtual void turnAngle(const QAngle& angle, const Turner& turner = pointTurn,
                         Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face a point.
   *
   * @param point   The point to seek.
   * @param turner  The turner that executes the turn.
   * @param settler The settler that tells the turn to stop.
   */
  virtual void turnToPoint(const Vector& point, const Turner& turner = pointTurn,
                           Settler&& settler = Settler().turnSettled());

  /**
   * Drive a linear distance while maintaining angle using an Angler. This method should not be
   * directly called, instead use moveDistance which creates an Angler to the current heading. This
   * uses the distance and angle PID controller.
   *
   * @param distance The linear distance to drive.
   * @param angler   The angler that keeps the drive straight.
   * @param settler  The settler that tells the drive to stop.
   */
  virtual void moveDistanceAtAngle(const QLength& distance, const Angler& angler,
                                   Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Drive a linear distance while maintaining starting angle.
   *
   * @param distance The linear distance to drive.
   * @param settler  The settler that tells the drive to stop.
   */
  virtual void moveDistance(const QLength& distance,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Drive to a point using custom point seeking. This method computes how much to turn and drive
   * depending on how the robot is facing the point. Stops correcting angle when driveRadius is
   * reached. This uses the distance and angle PID controller.
   *
   * @param point     The target point to drive to.
   * @param turnScale The turn scale is used to control the priority of turning over driving. A
   *                  higher value will make the robot turn to face the point sooner, and a lower
   *                  value will arc for longer.
   * @param settler   The settler that tells the drive to stop.
   */
  virtual void driveToPoint(const Vector& point, double turnScale = 1,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Get the state from the odometry in cartesian coordinates.
   *
   * @return The odometry state.
   */
  State getState() const;

  /**
   * Get the error of the distance PID controller. Is valid when driving linearly or driving to a
   * point.
   *
   * @return The distance error.
   */
  QLength getDistanceError() const;

  /**
   * Get the error of the angle or turn PID controller. Is valid when turning or driving.
   *
   * @return The angle error.
   */
  QAngle getAngleError() const;

  /**
   * Determine if the distance PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isDistanceSettled() const;

  /**
   * Determine if the angle PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isAngleSettled() const;

  /**
   * Determine if the turn PID controller is settled.
   *
   * @return true if settled, false otherwise.
   */
  bool isTurnSettled() const;

  /**
   * Make a Trigger that fires if the distance to a point is within a value.
   *
   * @param  point   The point.
   * @param  trigger The distance to the point.
   * @return A function that triggers when the distance to a point is within a value.
   */
  Trigger::Function distanceTo(const Vector& point, const QLength& trigger) const;

  /**
   * Make a Trigger that fires if the angle to a point is within a value.
   *
   * @param  point   The point.
   * @param  trigger The angle to the point.
   * @return A function that triggers when the angle to a point is within a value.
   */
  Trigger::Function angleTo(const Vector& point, const QAngle& trigger) const;

  /**
   * Make a Trigger that fires if the angle to an absolute angle is within a value.
   *
   * @param  angle   The absolute angle.
   * @param  trigger The angle to the absolute angle.
   * @return A function that triggers when the angle to an absolute angle is within a value.
   */
  Trigger::Function angleTo(const QAngle& angle, const QAngle& trigger) const;

  /**
   * Make a Trigger that fires if the distance error of the controller is within a value.
   *
   * @param  trigger The distance error.
   * @return A function that triggers when the distance error of the controller is within a value.
   */
  Trigger::Function distanceErr(const QLength& trigger) const;

  /**
   * Make a Trigger that fires if the angle error of the controller is within a value.
   *
   * @param  trigger The angle error.
   * @return A function that triggers when the angle error of the controller is within a value.
   */
  Trigger::Function angleErr(const QAngle& trigger) const;

  /**
   * Make a Trigger that fires if the distance controller is settled.
   *
   * @return A function that triggers when the distance controller is settled.
   */
  Trigger::Function distanceSettled() const;

  /**
   * Make a Trigger that fires if the turn controller is settled.
   *
   * @return A function that triggers when the turn controller is settled.
   */
  Trigger::Function turnSettled() const;

  /**
   * Make a Trigger that fires if the angle controller is settled.
   *
   * @return A function that triggers when the angle controller is settled.
   */
  Trigger::Function angleSettled() const;

  /**
   * Make a Trigger that fires if the distance error is settled according to a given settled util.
   * The error is in millimeters.
   *
   * @param  timeUtil A timeUtil containing a settled util.
   * @return A function that triggers when the distance controller is settled.
   */
  Trigger::Function distanceSettledUtil(const TimeUtil& timeUtil) const;

  /**
   * Make a Trigger that fires if the angle error is settled according to a given settled util. The
   * error is in degrees.
   *
   * @param  timeUtil A timeUtil containing a settled util.
   * @return A function that triggers when the angle controller is settled.
   */
  Trigger::Function angleSettledUtil(const TimeUtil& timeUtil) const;

protected:
  /**
   * Reset the pid controllers, used before every motion.
   */
  virtual void resetPid();

  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};
  std::unique_ptr<IterativePosPIDController> distanceController {nullptr};
  std::unique_ptr<IterativePosPIDController> angleController {nullptr};
  std::unique_ptr<IterativePosPIDController> turnController {nullptr};
  const QLength driveRadius;

  QLength _distanceErr {0_in};
  QAngle _angleErr {0_deg};
};
} // namespace lib7842
