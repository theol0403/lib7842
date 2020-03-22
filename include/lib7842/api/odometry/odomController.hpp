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
 * Function that accepts a turning velocity and controls execution to the chassis. Used to implement
 * a point or pivot turn.
 */
using Turner = std::function<void(ChassisModel& model, double vel)>;

/**
 * Function that returns an angle for the chassis to seek. Examples can be an AngleCalculator that
 * returns the angle to a point, or an angle to an absolute angle.
 */
using AngleCalculator = std::function<QAngle(const OdomController& odom)>;

/**
 * Odometry motion controller for skid-steer chassis.
 */
class OdomController {
public:
  /**
   * OdomController. Implements chassis movement algorithms.
   *
   * @param imodel              The chassis model
   * @param iodometry           The chassis odometry
   * @param idistanceController The distance pid controller
   * @param iturnController     The turning pid controller
   * @param iangleController    The angle pid controller, used to keep distance driving straight
   * @param idriveRadius        The radius from the target point to turn off angle correction when
   *                            driving to a point
   */
  OdomController(const std::shared_ptr<ChassisModel>& imodel,
                 const std::shared_ptr<Odometry>& iodometry,
                 std::unique_ptr<IterativePosPIDController> idistanceController,
                 std::unique_ptr<IterativePosPIDController> iturnController,
                 std::unique_ptr<IterativePosPIDController> iangleController,
                 const QLength& idriveRadius);

  virtual ~OdomController() = default;

  /**
   * Turn the chassis using the given AngleCalculator.
   *
   * @param angleCalculator The angle calculator
   * @param turner          The turner
   * @param settler         The settler
   */
  virtual void turn(const AngleCalculator& angleCalculator, const Turner& turner = pointTurn,
                    Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face an absolute angle.
   *
   * @param angle   The angle
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnToAngle(const QAngle& angle, const Turner& turner = pointTurn,
                           Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face a relative angle.
   *
   * @param angle   The angle
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnAngle(const QAngle& angle, const Turner& turner = pointTurn,
                         Settler&& settler = Settler().turnSettled());

  /**
   * Turn the chassis to face a point.
   *
   * @param point   The point
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnToPoint(const Vector& point, const Turner& turner = pointTurn,
                           Settler&& settler = Settler().turnSettled());

  /**
   * Drive a distance while mantaining angle using an AngleCalculator. This method should not be directly called, instead use moveDistance which creates an AngleCalculator to the current heading.
   *
   * @param distance        The distance
   * @param angleCalculator The angle calculator
   * @param turnScale       The turn scale
   * @param settler         The settler
   */
  virtual void moveDistanceAtAngle(const QLength& distance, const AngleCalculator& angleCalculator,
                                   double turnScale,
                                   Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Drive a distance while maintaining starting angle.
   *
   * @param distance The distance
   * @param settler  The settler
   */
  virtual void moveDistance(const QLength& distance,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Drive to a point using custom point seeking. This method computes how much to turn and drive depending on how the robot is facing the point.
   *
   * @param targetPoint The target point
   * @param turnScale   The turn scale used to control the priority of turning over driving. A
   *                    higher value will make the robot turn to face the point sooner
   * @param settler     The settler
   */
  virtual void driveToPoint(const Vector& targetPoint, double turnScale = 1,
                            Settler&& settler = Settler().distanceSettled().angleSettled());

  /**
   * Get the odometry state.
   */
  State getState() const;

  /**
   * Get distance from the chassis to the point.
   */
  QLength distanceToPoint(const Vector& point) const;

  /**
   * Get angle from the chassis to the point
   */
  QAngle angleToPoint(const Vector& point) const;

  /**
   * Get the error of the distance PID controller.
   */
  QLength getDistanceError() const;

  /**
   * Get the error of the angle or turn PID controller.
   */
  QAngle getAngleError() const;

  /**
   * Determine if the distance PID controller is settled.
   * @return true if settled, false otherwise
   */
  bool isDistanceSettled() const;

  /**
   * Determine if the angle PID controller is settled.
   * @return true if settled, false otherwise
   */
  bool isAngleSettled() const;

  /**
   * Determine if the turn PID controller is settled.
   * @return true if settled, false otherwise
   */
  bool isTurnSettled() const;

  /**
   * Make a Turner that executes a point turn which turns in place. Used as default for turn functions.
   */
  static void pointTurn(ChassisModel& model, double vel);

  /**
   * Make a Turner that executes a left pivot, meaning it only moves the left motors.
   */
  static void leftPivot(ChassisModel& model, double vel);

  /**
   * Make a Turner that executes a right pivot, meaning it only moves the right motors.
   */
  static void rightPivot(ChassisModel& model, double vel);

  /**
   * Make an AngleCalculator that seeks an absolute angle.
   *
   * @param angle The angle to seek.
   */
  static AngleCalculator makeAngleCalculator(const QAngle& angle);

  /**
   * Make an AngleCalculator that seeks a point.
   *
   * @param point The point to seek.
   */
  static AngleCalculator makeAngleCalculator(const Vector& point);

  /**
   * Make an AngleCalculator that does nothing. This is the default AngleCalculator.
   */
  static AngleCalculator makeAngleCalculator();

  /**
   * Settler if the distance to a point is within a value.
   *
   * @param point   The point
   * @param Settler The distance to the point.
   */
  std::function<bool()> distanceTo(const Vector& point, const QLength& Settler);

  /**
   * Settler if the angle to a point is within a value.
   *
   * @param point   The point
   * @param Settler The angle to the point.
   */
  std::function<bool()> angleTo(const Vector& point, const QAngle& Settler);

  /**
   * Settler if the angle to an absolute angle is within a value.
   *
   * @param angle   The absolute angle
   * @param Settler The angle to the absolute angle.
   */
  std::function<bool()> angleTo(const QAngle& angle, const QAngle& Settler);

  /**
   * Settler if the distance error of the controller is within a value.
   *
   * @param Settler The distance error.
   */
  std::function<bool()> distanceErr(const QLength& Settler);

  /**
   * Settler if the angle error of the controller is within a value.
   *
   * @param Settler The angle error.
   */
  std::function<bool()> angleErr(const QAngle& Settler);

  /**
   * Settler if the distance controller is settled.
   */
  std::function<bool()> distanceSettled();

  /**
   * Settler if the turn controller is settled.
   */
  std::function<bool()> turnSettled();

  /**
   * Settler if the angle controller is settled.
   */
  std::function<bool()> angleSettled();

  /**
   * Settler if the distance error is settled according to a given settled util. The error is in
   * millimeters.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  std::function<bool()> distanceSettledUtil(const TimeUtil& timeUtil);

  /**
   * Settler if the angle error is settled according to a given settled util. The error is in degrees.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  std::function<bool()> angleSettledUtil(const TimeUtil& timeUtil);

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

  QLength _distanceErr = 0_in;
  QAngle _angleErr = 0_deg;
};
} // namespace lib7842
