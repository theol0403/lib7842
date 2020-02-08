#pragma once
#include "lib7842/api/odometry/odomController.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include <functional>

namespace lib7842 {

class Trigger {
public:
  Trigger() = default;
  Trigger(const OdomController* icontroller);
  virtual ~Trigger() = default;

  /**
   * Requirements
   * The trigger will only fire if all requirements are met.
   */
  virtual void isTrue(const std::function<bool()>& function);

  virtual void distanceTo(const Vector& point, const QLength& trigger);
  virtual void angleTo(const Vector& point, const QAngle& trigger);
  virtual void angleTo(const QAngle& angle, const QAngle& trigger);

  virtual void distanceErr(const QLength& trigger);
  virtual void angleErr(const QAngle& trigger);

  virtual void distanceSettled();
  virtual void turnSettled();
  virtual void angleSettled();

  // void distanceSettledUtil(const TimeUtil& timeUtil);
  // void angleSettledUtil(const TimeUtil& timeUtil);
  // void turnSettledUtil(const TimeUtil& timeUtil);

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
  std::vector<std::function<bool()>> exeptions;
};
} // namespace lib7842