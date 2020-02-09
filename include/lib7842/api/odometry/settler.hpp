#pragma once
#include "okapi/api/units/QSpeed.hpp"
#include "trigger.hpp"

namespace lib7842 {

class Settler : public Trigger {
public:
  using Trigger::Trigger;
  Settler(Trigger&& trigger);

  Settler&& abort(const TimeUtil& itimeUtil);
  Settler&& noAbort();

  bool operator()() override;

  static void setDefaultAbort(const TimeUtil& itimeUtil);

protected:
  std::shared_ptr<SettledUtil> driveAbort {defaultAbort};
  static std::shared_ptr<SettledUtil> defaultAbort;

  QLength lastError {0_m};
};
} // namespace lib7842