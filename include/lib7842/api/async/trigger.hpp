#pragma once
#include "lib7842/api/other/global.hpp"
#include <functional>

namespace lib7842 {
using namespace okapi;

/**
 * A Trigger is a collection of functions. When all requirements are met, or
 * any exceptions are met, then the Trigger will return true. Used for determining when to settle
 * for PID movements and used for triggering async actions.
 */
class Trigger {
public:
  Trigger() = default;
  Trigger(const Trigger&) = delete;
  Trigger(Trigger&&) = default;
  virtual ~Trigger() = default;

  /**
   * Add a requirement.
   * The trigger will only fire if all requirements are met.
   *
   * @param function The requirement
   */
  virtual Trigger&& requirement(std::function<bool()>&& function);
  virtual Trigger&& needs(std::function<bool()>&& function);

  /**
   * Add an exception.
   * The trigger will fire if any of the exceptions are met.
   *
   * @param function The exception
   */
  virtual Trigger&& exception(std::function<bool()>&& function);
  virtual Trigger&& unless(std::function<bool()>&& function);

  /**
   * Run all the requirements and exceptions.
   *
   * @return Whether the trigger has been fired
   */
  virtual bool run();
  virtual bool operator()();

  /**
   * Trigger if the time since the first call of the function is greater than a value.
   *
   * @param time The time
   */
  static std::function<bool()> timePassed(const QTime& time);

protected:
  std::vector<std::function<bool()>> requirements;
  std::vector<std::function<bool()>> exceptions;
};
} // namespace lib7842
