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
  explicit Trigger(const OdomController* icontroller);

  /**
   * Requirements
   * The trigger will only fire if all requirements are met.
   */
  virtual Trigger&& requirement(std::function<bool()>&& function);
  virtual Trigger&& exception(std::function<bool()>&& function);

  virtual Trigger&& distanceTo(const Vector& point, const QLength& trigger);
  virtual Trigger&& angleTo(const Vector& point, const QAngle& trigger);
  virtual Trigger&& angleTo(const QAngle& angle, const QAngle& trigger);

  virtual Trigger&& distanceErr(const QLength& trigger);
  virtual Trigger&& angleErr(const QAngle& trigger);

  virtual Trigger&& distanceSettled();
  virtual Trigger&& turnSettled();
  virtual Trigger&& angleSettled();

  virtual Trigger&& distanceSettledUtil(const TimeUtil& timeUtil);
  virtual Trigger&& angleSettledUtil(const TimeUtil& timeUtil);

  /**
   * Exceptions
   * The trigger will fire if any of the exceptions are met.
   */
  virtual Trigger&& maxTime(const QTime& time, const TimeUtil& timeUtil);

  /**
   * Run all the requirements and exceptions, while providing a controller
   *
   * @param  icontroller The controller
   * @return Whether the trigger has been fired
   */
  virtual bool operator()(const OdomController* icontroller);

  /**
   * Run all the requirements and exceptions. The controller is assumed to be provided by the
   * constructor.
   *
   * @return Whether the trigger has been fired
   */
  virtual bool operator()();

protected:
  const OdomController* controller {nullptr};

  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exceptions;
};
} // namespace lib7842