#pragma once
#include "lib7842/api/async/trigger.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QLength.hpp"
#include <stack>

namespace lib7842 {

// forward declaration
class OdomController;

/**
 * Settler is a Trigger that is specific to driving. The controller is passed when the trigger is
 * run, to allow for static method chaining.
 *
 * A Settler has an emergency abort which detects if the robot is stuck. It uses a SettledUtil on
 * the derivative of an OdomController's distance error to abort when the distance error does not
 * change for a long time.
 *
 * By default, Settlers have no abort. However, it is possible to change the default for all new
 * Settlers or provide a one-time abort.
 */
class Settler : public Trigger {
public:
  using Trigger::Trigger;

  /**
   * Require that the distance to a point is within a value.
   *
   * @param point   The point
   * @param trigger The distance to the point.
   */
  virtual Settler&& distanceTo(const Vector& point, const QLength& trigger);

  /**
   * Require that the angle to a point is within a value.
   *
   * @param point   The point
   * @param trigger The angle to the point.
   */
  virtual Settler&& angleTo(const Vector& point, const QAngle& trigger);

  /**
   * Require that the angle to an absolute angle is within a value.
   *
   * @param angle   The absolute angle
   * @param trigger The angle to the absolute angle.
   */
  virtual Settler&& angleTo(const QAngle& angle, const QAngle& trigger);

  /**
   * Require that the distance error of the controller is within a value.
   *
   * @param trigger The distance error.
   */
  virtual Settler&& distanceErr(const QLength& trigger);

  /**
   * Require that the angle error of the controller is within a value.
   *
   * @param trigger The angle error.
   */
  virtual Settler&& angleErr(const QAngle& trigger);

  /**
   * Require that the distance controller is settled.
   */
  virtual Settler&& distanceSettled();

  /**
   * Require that the turn controller is settled.
   */
  virtual Settler&& turnSettled();

  /**
   * Require that the angle controller is settled.
   */
  virtual Settler&& angleSettled();

  /**
   * Require that the distance error is settled according to a given settled util. The error is in
   * millimeters.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  virtual Settler&& distanceSettledUtil(const TimeUtil& timeUtil);

  /**
   * Require that the angle error is settled according to a given settled util. The error is in
   * degrees.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  virtual Settler&& angleSettledUtil(const TimeUtil& timeUtil);

  /**
   * Make an exception if the derivative of the distance error in millimeters is within the ranges
   * of a settled util.
   *
   * @param itimeUtil The timeUtil containing a settled util.
   */
  virtual Settler&& abort(const TimeUtil& itimeUtil);

  /**
   * Remove any abort that has been set. Automatically gets called by turn commands, as aborts are
   * only for the distance domain.
   */
  virtual Settler&& noAbort();

  /**
   * Check the abort, then run all the requirements and exceptions.
   *
   * @return Whether the settler has been fired.
   */
  virtual bool run(const OdomController* icontroller);
  virtual bool operator()(const OdomController* icontroller);

  /**
   * Set the default abort parameters for all new Settler objects.
   *
   * @param itimeUtil The timeUtil containing a settled util.
   */
  static void setDefaultAbort(const TimeUtil& itimeUtil);

protected:
  using Trigger::run; // use the version with the controller

  std::stack<std::function<void()>> assembly {};

  const OdomController* controller {nullptr};
  std::shared_ptr<SettledUtil> driveAbort {defaultAbort};
  QLength lastError {0_m};

  static std::shared_ptr<SettledUtil> defaultAbort;
};
} // namespace lib7842
