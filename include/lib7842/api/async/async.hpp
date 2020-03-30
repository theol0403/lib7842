#pragma once
#include "lib7842/api/other/global.hpp"
#include "taskWrapper.hpp"
#include "trigger.hpp"

namespace lib7842 {

/**
 * A helper class for running actions asynchronously.
 */
class Async : public TaskWrapper {
public:
  Async(const Async& itask) = delete;
  Async(Async&& itask) = default;

  /**
   * Run a function asynchronously.
   *
   * @param iaction The function to run.
   */
  explicit Async(std::function<void()>&& iaction);

  /**
   * Run a function asynchronously after a trigger fires.
   *
   * @param itrigger The trigger to wait for before running the action.
   * @param iaction  The function to run.
   */
  explicit Async(Trigger&& itrigger, std::function<void()>&& iaction);

  /**
   * If a trigger was specified, force it to fire and run the action.
   */
  virtual void forceStart();

  /**
   * Stop the internal task.
   */
  virtual void forceStop();

  /**
   * Whether the action has been started.
   *
   * @return True if started, False otherwise.
   */
  virtual bool hasStarted() const;

  /**
   * Whether the action has completed and returned.
   *
   * @return True if the action is complete, False otherwise.
   */
  virtual bool isComplete() const;

  /**
   * Wait until the action is complete.
   */
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
