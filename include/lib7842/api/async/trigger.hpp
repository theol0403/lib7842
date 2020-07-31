#pragma once
#include "lib7842/api/other/global.hpp"
#include <functional>

namespace lib7842 {
using namespace okapi;

/**
 * A Trigger is a collection of functions. When all requirements are met, or any exceptions are met,
 * then the Trigger will return true. Used for determining when to settle for PID movements and
 * triggering async actions.
 */
class Trigger {
public:
  /**
   * A function wrapper that supports the not operator.
   */
  class Function : public std::function<bool()> {
  public:
    using function::function;
    Function operator!() &&;
  };

  /**
   * Trigger constructors.
   */
  Trigger() = default;
  Trigger(const Trigger&) = delete;
  Trigger operator=(const Trigger&) = delete;
  Trigger(Trigger&&) = default;
  Trigger& operator=(Trigger&&) = default;
  virtual ~Trigger() = default;

  /**
   * Add a requirement. The trigger will only fire if all requirements are met.
   *
   * @param function The requirement
   */
  virtual Trigger&& requirement(Function&& function);
  virtual Trigger&& require(Function&& function);

  /**
   * Add an exception. The trigger will fire if any of the exceptions are met.
   *
   * @param function The exception
   */
  virtual Trigger&& exception(Function&& function);
  virtual Trigger&& unless(Function&& function);

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
   * @param  time The time
   * @return A function that triggers when the time since the first call of the function is greater
   *         than a value.
   */
  static Function time(const QTime& time);

protected:
  std::vector<Function> requirements;
  std::vector<Function> exceptions;
};
} // namespace lib7842
