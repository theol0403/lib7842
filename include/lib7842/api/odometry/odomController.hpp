#pragma once
#include "lib7842/api/other/utility.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include <functional>

namespace lib7842 {
using namespace okapi;
class OdomController;

/**
 * Function that returns true to end chassis movement. Used to implement different settling methods.
 */
using Settler = std::function<bool(const OdomController& odom)>;

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
   * OdomController. Implements chassis movement algorithms
   *
   * @param imodel              The chassis model
   * @param iodometry           The chassis odometry
   * @param idistanceController The distance pid controller
   * @param iturnController     The turning pid controller
   * @param iangleController    The angle pid controller, used to keep distance driving straight
   * @param isettleRadius       The radius from the target point to give up angle correction
   * @param itimeUtil           The time utility
   */
  OdomController(const std::shared_ptr<ChassisModel>& imodel,
                 const std::shared_ptr<Odometry>& iodometry,
                 std::unique_ptr<IterativePosPIDController> idistanceController,
                 std::unique_ptr<IterativePosPIDController> iturnController,
                 std::unique_ptr<IterativePosPIDController> iangleController,
                 const QLength& isettleRadius, const TimeUtil& itimeUtil);

  virtual ~OdomController() = default;

  /**
   * Turn the chassis using the given AngleCalculator
   *
   * @param angleCalculator The angle calculator
   * @param turner          The turner
   * @param settler         The settler
   */
  virtual void turn(const AngleCalculator& angleCalculator, const Turner& turner = pointTurn,
                    const Settler& settler = defaultTurnSettler);

  /**
   * Turn the chassis to face an absolute angle
   *
   * @param angle   The angle
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnToAngle(const QAngle& angle, const Turner& turner = pointTurn,
                           const Settler& settler = defaultTurnSettler);

  /**
   * Turn the chassis to face a relative angle
   *
   * @param angle   The angle
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnAngle(const QAngle& angle, const Turner& turner = pointTurn,
                         const Settler& settler = defaultTurnSettler);

  /**
   * Turn the chassis to face a point
   *
   * @param point   The point
   * @param turner  The turner
   * @param settler The settler
   */
  virtual void turnToPoint(const Vector& point, const Turner& turner = pointTurn,
                           const Settler& settler = defaultTurnSettler);

  /**
   * Drive a distance while correcting angle using an AngleCalculator
   *
   * @param distance        The distance
   * @param angleCalculator The angle calculator
   * @param turnScale       The turn scale
   * @param settler         The settler
   */
  virtual void moveDistanceAtAngle(const QLength& distance, const AngleCalculator& angleCalculator,
                                   double turnScale,
                                   const Settler& settler = defaultDriveAngleSettler);

  /**
   * Drive a distance while maintaining starting angle
   *
   * @param distance The distance
   * @param settler  The settler
   */
  virtual void moveDistance(const QLength& distance,
                            const Settler& settler = defaultDriveAngleSettler);

  /**
   * Drive to a point using custom point seeking
   *
   * @param targetPoint The target point
   * @param turnScale   The turn scale used to control the priority of turning over driving. A
   *                    higher value will make the robot turn to face the point sooner
   * @param settler     The settler
   */
  virtual void driveToPoint(const Vector& targetPoint, double turnScale = 1,
                            const Settler& settler = defaultDriveAngleSettler);

  /**
   * Drive to a point using simple point seeking
   *
   * @param targetPoint The target point
   * @param turnScale   The turn scale used to control the priority of turning over driving. A
   *                    higher value will make the robot turn to face the point sooner
   * @param settler     The settler
   */
  virtual void driveToPoint2(const Vector& targetPoint, double turnScale = 1,
                             const Settler& settler = defaultDriveAngleSettler);

  /**
   * A Settler that is used for turning which uses the turning pid's isSettled() method
   */
  static bool defaultTurnSettler(const OdomController& odom);

  /**
   * A Settler that is used for driving which uses the distance pid's isSettled() method
   */
  static bool defaultDriveSettler(const OdomController& odom);

  /**
   * A Settler that is used for driving which uses the distance and angle pid's isSettled() method
   */
  static bool defaultDriveAngleSettler(const OdomController& odom);

  /**
   * A Turner that executes a point turn which turns in place. Used as default for turn functions
   */
  static void pointTurn(ChassisModel& model, double vel);

  /**
   * A Turner that executes a left pivot, meaning it only moves the left motors.
   */
  static void leftPivot(ChassisModel& model, double vel);

  /**
   * A Turner that executes a right pivot, meaning it only moves the right motors.
   */
  static void rightPivot(ChassisModel& model, double vel);

  /**
   * Make a Settler that exits when angle error is within given range
   * @param angle The angle error threshold
   */
  static Settler makeSettler(const QAngle& angle);

  /**
   * Make a Settler that exits when distance error is within given range
   * @param distance The distance error threshold
   */
  static Settler makeSettler(const QLength& distance);

  /**
   * Make a Settler that exits when both angle and distance error is within given range.
   * @param angle The angle error threshold
   * @param distance The distance error threshold
   */
  static Settler makeSettler(const QLength& distance, const QAngle& angle);

  /**
   * Make an AngleCalculator that seeks a given absolute angle
   *
   * @param angle The angle
   */
  static AngleCalculator makeAngleCalculator(const QAngle& angle);

  /**
   * Make an AngleCaclulator that seeks a given point.
   *
   * @param point The point
   */
  static AngleCalculator makeAngleCalculator(const Vector& point);

  /**
   * Make an AngleCaclulator that returns a constant error. The default settler needs to be changed
   * for a command using this calculator to settle.
   *
   * @param  error The error
   * @return The angle calculator.
   */
  static AngleCalculator makeAngleCalculator(double error);

  /**
   * Make an AngleCalculator that does nothing
   */
  static AngleCalculator makeAngleCalculator();

  /**
   * Get the odometry state.
   */
  State getState() const;

  /**
   * Calculate distance from the chassis to the point
   */
  QLength distanceToPoint(const Vector& point) const;

  /**
   * Calculate angle from the chassis to the point
   */
  QAngle angleToPoint(const Vector& point) const;

protected:
  /**
   * Reset the pid controllers, used before every motion
   */
  virtual void resetPid();

  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};
  std::unique_ptr<IterativePosPIDController> distanceController {nullptr};
  std::unique_ptr<IterativePosPIDController> angleController {nullptr};
  std::unique_ptr<IterativePosPIDController> turnController {nullptr};
  const QLength settleRadius;
  TimeUtil timeUtil;

  QAngle angleErr = 0_deg;
  QLength distanceErr = 0_in;
};
} // namespace lib7842
