#pragma once
#include "trigger.hpp"

namespace lib7842 {

/**
 * Settler is a Trigger that has an emergency abort which can be added or removed. It uses a
 * SettledUtil on the derivative of an OdomController's distance error. Used to abort a movement
 * when the distance error does not change for a long time.
 *
 * By default, Settlers have no abort. However, it is possible to specify the default abort
 * parameters, as well override individual aborts.
 */
class Settler : public Trigger {
public:
  using Trigger::Trigger;

  /**
   * Make an exception if the derivative of the distance error is within the ranges of a settled
   * util.
   *
   * @param  itimeUtil The timeUtil containing a settled util.
   */
  Settler&& abort(const TimeUtil& itimeUtil);

  /**
   * Remove any abort that has been set. Automatically gets called by turn commands.
   */
  Settler&& noAbort() override;

  /**
   * Check the abort, then run all the requirements and exceptions.
   *
   * @return Whether the settler has been fired.
   */
  bool run() override;

  /**
   * Set the default abort parameters for all new Settler objects.
   *
   * @param itimeUtil The timeUtil containing a settled util.
   */
  static void setDefaultAbort(const TimeUtil& itimeUtil);

protected:
  std::shared_ptr<SettledUtil> driveAbort {defaultAbort};
  static std::shared_ptr<SettledUtil> defaultAbort;

  QLength lastError {0_m};
};
} // namespace lib7842