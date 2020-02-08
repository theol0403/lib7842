#pragma once
#include "odomController.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {

class Trigger {
public:
  using Abort = std::function<bool(const OdomController*)>;

  Trigger(const OdomController* icontroller);

  /**
   * Requirements
   */
  void distanceTo(const Vector& point, const QLength& trigger);
  void angleTo(const Vector& point, const QAngle& trigger);
  void angleTo(const QAngle& angle, const QAngle& trigger);

  void distanceErr(const QLength& trigger);
  void angleErr(const QAngle& trigger);

  void distanceSettled();
  void turnSettled();
  void angleSettled();

  // void distanceSettledUtil(const TimeUtil& timeUtil);
  // void angleSettledUtil(const TimeUtil& timeUtil);
  // void turnSettledUtil(const TimeUtil& timeUtil);

  void isTrue(const std::function<bool()>& function);

  /**
   * Exceptions
   */
  void maxTime(const QTime& time);

  /**
   * Abort
   */
  void withAbort(const QSpeed& threshold, const QTime& time);
  void noAbort();

protected:
  const OdomController* controller {nullptr};

  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exeptions;
  Abort abort {defaultAbort};

  static Abort defaultAbort;
};
} // namespace lib7842