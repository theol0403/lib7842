#pragma once
#include "lib7842/api/async/trigger.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QLength.hpp"

namespace lib7842 {

// forward declaration
class OdomController;

/**
 * Settler is a Trigger that has an emergency abort which can be added or removed. It uses a
 * SettledUtil on the derivative of an OdomController's distance error. Used to abort a movement
 * when the distance error does not change for a long time.
 *
 * By default, Settlers have no abort. However, it is possible to specify the default abort
 * parameters, as well override individual aborts.
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
  Settler&& distanceTo(const Vector& point, const QLength& trigger);

  /**
   * Require that the angle to a point is within a value.
   *
   * @param point   The point
   * @param trigger The angle to the point.
   */
  Settler&& angleTo(const Vector& point, const QAngle& trigger);

  /**
   * Require that the angle to an absolute angle is within a value.
   *
   * @param angle   The absolute angle
   * @param trigger The angle to the absolute angle.
   */
  Settler&& angleTo(const QAngle& angle, const QAngle& trigger);

  /**
   * Require that the distance error of the controller is within a value.
   *
   * @param trigger The distance error.
   */
  Settler&& distanceErr(const QLength& trigger);

  /**
   * Require that the angle error of the controller is within a value.
   *
   * @param trigger The angle error.
   */
  Settler&& angleErr(const QAngle& trigger);

  /**
   * Require that the distance controller is settled.
   */
  Settler&& distanceSettled();

  /**
   * Require that the turn controller is settled.
   */
  Settler&& turnSettled();

  /**
   * Require that the angle controller is settled.
   */
  Settler&& angleSettled();

  /**
   * Require that the distance error is settled according to a given settled util. The error is in
   * millimeters.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  Settler&& distanceSettledUtil(const TimeUtil& timeUtil);

  /**
   * Require that the angle error is settled according to a given settled util. The error is in degrees.
   *
   * @param timeUtil A timeUtil containing a settled util.
   */
  Settler&& angleSettledUtil(const TimeUtil& timeUtil);

  /**
   * Make an exception if the derivative of the distance error is within the ranges of a settled
   * util.
   *
   * @param itimeUtil The timeUtil containing a settled util.
   */
  Settler&& abort(const TimeUtil& itimeUtil);

  /**
   * Remove any abort that has been set. Automatically gets called by turn commands.
   */
  Settler&& noAbort();

  /**
   * Set the default abort parameters for all new Settler objects.
   *
   * @param itimeUtil The timeUtil containing a settled util.
   */
  static void setDefaultAbort(const TimeUtil& itimeUtil);

  /**
   * Check the abort, then run all the requirements and exceptions.
   *
   * @return Whether the settler has been fired.
   */
  bool run(const OdomController* icontroller);

protected:
  using Trigger::run;
  const OdomController* controller {nullptr};

  std::shared_ptr<SettledUtil> driveAbort {defaultAbort};
  static std::shared_ptr<SettledUtil> defaultAbort;

  QLength lastError {0_m};
};
} // namespace lib7842
