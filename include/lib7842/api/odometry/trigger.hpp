#pragma once
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include <functional>

namespace lib7842 {

class OdomController; // forward declare
using namespace okapi;

/**
 * A Trigger that returns true when certain requirements and exceptions are met. Used for settling
 * and async actions.
 */
class Trigger {
public:
  Trigger() = default;
  Trigger(const Trigger&) = delete;
  Trigger(Trigger&&) = default;
  virtual ~Trigger() = default;

  /**
   * Create a new Trigger object. Provides the controller pointer for later use.
   *
   * @param icontroller The icontroller
   */
  explicit Trigger(const OdomController* icontroller);

  /**
   * Add a requirement
   * The trigger will only fire if all requirements are met.
   *
   * @param  function The requirement
   */
  virtual Trigger&& requirement(std::function<bool(const OdomController* icontroller)>&& function);

  /**
   * Add an exception
   * The trigger will fire if any of the exceptions are met.
   *
   * @param  function The exception
   */
  virtual Trigger&& exception(std::function<bool(const OdomController* icontroller)>&& function);

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
   * @param  point   The absolute angle
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
   * @param  time     The time
   * @param  timeUtil The timeUtil for keeping track of time.
   */
  virtual Trigger&& maxTime(const QTime& time, const TimeUtil& timeUtil);

  /**
   * Run all the requirements and exceptions. The controller is assumed to be provided by the
   * constructor.
   *
   * @return Whether the trigger has been fired
   */
  virtual bool run();
  virtual bool operator()();

  /**
   * Run all the requirements and exceptions, while providing a controller
   *
   * @param  icontroller The controller
   * @return Whether the trigger has been fired
   */
  virtual bool run(const OdomController* icontroller);
  virtual bool operator()(const OdomController* icontroller);

protected:
  const OdomController* controller {nullptr};

  std::vector<std::function<bool(const OdomController* icontroller)>> requirements;
  std::vector<std::function<bool(const OdomController* icontroller)>> exceptions;
};
} // namespace lib7842