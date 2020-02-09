#pragma once
#include "okapi/api/units/QSpeed.hpp"
#include "trigger.hpp"

namespace lib7842 {

class Settler : public Trigger {
public:
  using Trigger::Trigger;

  void abort(const TimeUtil& itimeUtil);
  void noAbort();

  static void setDefaultAbort(const TimeUtil& itimeUtil);

protected:
  std::function<bool()> driveAbort {defaultAbort};
  static std::function<bool()> defaultAbort;
};
} // namespace lib7842