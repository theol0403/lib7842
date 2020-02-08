#pragma once
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/QTime.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include <functional>

namespace lib7842 {

class Trigger {
public:
  Trigger() = default;
  virtual ~Trigger() = default;

  /**
   * Requirements
   * The trigger will only fire if all requirements are met.
   */
  virtual void isTrue(const std::function<bool()>& function);

  virtual void distanceTo(const Vector& point, const QLength& trigger);
  virtual void angleTo(const Vector& point, const QAngle& trigger);
  virtual void angleTo(const QAngle& angle, const QAngle& trigger);

  /**
   * Exceptions
   * The trigger will fire if any of the exceptions are met.
   */
  virtual void maxTime(const QTime& time, const TimeUtil& itimeUtil);

  /**
   * Run all the requirements and exceptions
   *
   * @return Whether the trigger has been fired
   */
  virtual bool operator()();

protected:
  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exeptions;
};
} // namespace lib7842