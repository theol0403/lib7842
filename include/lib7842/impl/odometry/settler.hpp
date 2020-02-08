#pragma once
#include "lib7842/api/odometry/odomController.hpp"
#include "lib7842/api/odometry/trigger.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {

class Settler : public Trigger {
public:
  using Abort = std::function<bool(const OdomController*)>;

  Settler() = default;
  Settler(const OdomController* icontroller);

  /**
   * Requirements
   * The trigger will only fire if all requirements are met.
   */
  void distanceErr(const QLength& trigger);
  void angleErr(const QAngle& trigger);

  void distanceSettled();
  void turnSettled();
  void angleSettled();

  // void distanceSettledUtil(const TimeUtil& timeUtil);
  // void angleSettledUtil(const TimeUtil& timeUtil);
  // void turnSettledUtil(const TimeUtil& timeUtil);

  /**
   * Exceptions
   * The trigger will fire if any of the exceptions are met.
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