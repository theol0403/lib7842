#pragma once

#include "okapi/api/coreProsAPI.hpp"
#include "okapi/api/util/logging.hpp"
#include "pros/rtos.hpp"
#include <memory>

namespace lib7842 {

using namespace okapi;

/**
 * A utility class that wraps a task trampoline. To use, simply inherit your class from TaskWrapper
 * and override the `loop` method. To start the task, the `startTask` method must be called, either
 * from the constructor or from outside the class.
 */
class TaskWrapper {
protected:
  explicit TaskWrapper(const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());
  virtual ~TaskWrapper() = default;

  /**
   * Override this function to implement a custom task loop.
   * Will throw if not overridden.
   */
  virtual void loop();

public:
  /**
   * Starts the task.
   *
   * @param iname The task name, optional.
   */
  virtual void startTask(const std::string& iname = "TaskWrapper");

  /**
   * Kills the task.
   */
  virtual void killTask();

  /**
   * Gets the task name.
   *
   * @return The name.
   */
  virtual std::string getName();

protected:
  std::shared_ptr<Logger> logger {nullptr};

private:
  static void trampoline(void* iparam);
  std::unique_ptr<CrossplatformThread> task {nullptr};
};

} // namespace lib7842