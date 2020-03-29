#pragma once
#include "lib7842/api/other/global.hpp"
#include "taskWrapper.hpp"
#include "trigger.hpp"

namespace lib7842 {

class Async : public TaskWrapper {
public:
  Async(const Async& itask) = delete;
  Async(Async&& itask) = default;

  explicit Async(std::function<void()>&& iaction);
  explicit Async(Trigger&& itrigger, std::function<void()>&& iaction);

  virtual void forceStart();
  virtual void forceStop();

  virtual bool hasStarted() const;
  virtual bool isComplete() const;
  virtual void waitUntilComplete() const;

protected:
  Trigger trigger {};
  std::function<void()> action {nullptr};

  bool _forceStart {false};
  bool _started {false};
  bool _complete {false};

  void loop() override;
};
} // namespace lib7842
