#pragma once
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include <functional>

namespace lib7842 {

class OdomController; // forward declare
using namespace okapi;

/**
 * A Trigger is a collection of functions that act as triggers. When all requirements are met, or
 * any exceptions are met, then the Trigger will return true. Used for determining when to settle
 * for PID movements and used for triggering async actions.
 *
 * A Trigger must point to a OdomController when the run() method is called. This pointer is passed
 * when calling run().
 */
class Trigger {
public:
  Trigger() = default;
  Trigger(const Trigger&) = delete;
  Trigger(Trigger&&) = default;
  virtual ~Trigger() = default;

  /**
   * Add a requirement
   * The trigger will only fire if all requirements are met.
   *
   * @param  function The requirement
   */
  virtual Trigger&& requirement(std::function<bool()>&& function);

  /**
   * Add an exception
   * The trigger will fire if any of the exceptions are met.
   *
   * @param  function The exception
   */
  virtual Trigger&& exception(std::function<bool()>&& function);

  /**
   * Require that the distance to a point is within a value.
   *
   * @param  point   The point
   * @param  trigger The distance
   */
  virtual Trigger&& distanceTo(const Vector& point, const QLength& trigger);

  /**
   * Require that the angle to a point is within a value.
   *
   * @param  point   The point
   * @param  trigger The angle
   */
  virtual Trigger&& angleTo(const Vector& point, const QAngle& trigger);

  /**
   * Require that the angle to an absolute angle is within a value.
   *
   * @param  angle   The absolute angle
   * @param  trigger The angle
   */
  virtual Trigger&& angleTo(const QAngle& angle, const QAngle& trigger);

  /**
   * Require that the distance error of the controller is within a value.
   *
   * @param  trigger The distance
   */
  virtual Trigger&& distanceErr(const QLength& trigger);

  /**
   * Require that the angle error of the controller is within a value.
   *
   * @param  trigger The angle
   */
  virtual Trigger&& angleErr(const QAngle& trigger);

  /**
   * Require that the distance controller is settled.
   */
  virtual Trigger&& distanceSettled();

  /**
   * Require that the turn controller is settled.
   */
  virtual Trigger&& turnSettled();

  /**
   * Require that the angle controller is settled.
   */
  virtual Trigger&& angleSettled();

  /**
   * Require that the distance error is settled according to a settled util. The error is in
   * millimeters.
   *
   * @param  timeUtil The timeUtil containing a settled util.
   */
  virtual Trigger&& distanceSettledUtil(const TimeUtil& timeUtil);

  /**
   * Require that the angle error is settled according to a settled util. The error is in degrees.
   *
   * @param  timeUtil The timeUtil containing a settled util.
   */
  virtual Trigger&& angleSettledUtil(const TimeUtil& timeUtil);

  /**
   * Make an exception if the time from the first call of the trigger is greater than a value.
   *
   * @param  time The time
   */
  virtual Trigger&& maxTime(const QTime& time);

  /**
   * Remove any abort that has been set by a Settler. Automatically gets called by turn commands.
   */
  virtual Trigger&& noAbort();

  /**
   * Run all the requirements and exceptions.
   *
   * @param  icontroller The controller to run the requirements on.
   * @return Whether the trigger has been fired
   */
  virtual bool run(const OdomController* icontroller);
  virtual bool operator()(const OdomController* icontroller);

protected:
  /**
   * Run all the requirements and exceptions. Assumes the controller has already been passed.
   *
   * @return Whether the trigger has been fired
   */
  virtual bool run();

  const OdomController* controller {nullptr};

  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exceptions;
};
} // namespace lib7842
