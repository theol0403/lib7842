#pragma once
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include <functional>

namespace lib7842 {

class OdomController; // forward declare
using namespace okapi;

class Trigger {
public:
  Trigger() = default;
  virtual ~Trigger() = default;
  Trigger(const OdomController* icontroller);

  /**
   * Requirements
   * The trigger will only fire if all requirements are met.
   */
  virtual void requirement(std::function<bool()>&& function);
  virtual void exception(std::function<bool()>&& function);

  virtual void distanceTo(const Vector& point, const QLength& trigger);
  virtual void angleTo(const Vector& point, const QAngle& trigger);
  virtual void angleTo(const QAngle& angle, const QAngle& trigger);

  virtual void distanceErr(const QLength& trigger);
  virtual void angleErr(const QAngle& trigger);

  virtual void distanceSettled();
  virtual void turnSettled();
  virtual void angleSettled();

  virtual void distanceSettledUtil(const TimeUtil& timeUtil);
  virtual void angleSettledUtil(const TimeUtil& timeUtil);

  /**
   * Exceptions
   * The trigger will fire if any of the exceptions are met.
   */
  virtual void maxTime(const QTime& time, const TimeUtil& timeUtil);

  /**
   * Run all the requirements and exceptions
   *
   * @return Whether the trigger has been fired
   */
  virtual bool operator()();

  /**
   * Run all the requirements and exceptions, while providing a controller
   *
   * @param  icontroller The controller
   * @return Whether the trigger has been fired
   */
  virtual bool operator()(const OdomController* icontroller);

protected:
  const OdomController* controller {nullptr};

  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exceptions;
};
} // namespace lib7842