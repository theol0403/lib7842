#pragma once
#include "main.h"

namespace lib7842 {
class TaskWrapper {
protected:
  TaskWrapper(std::shared_ptr<Logger> ilogger = Logger::getDefaultLogger());
  virtual ~TaskWrapper() = default;

  /**
   * Extend this function to implement custom task.
   */
  virtual void loop();

public:
  /**
   * Starts the task.
   */
  virtual void startTask(const std::string& iname = "TaskWrapper");

  /**
   * Kills the task.
   */
  virtual void killTask();

  /**
   * Gets the task name.
   */
  virtual std::string getName();

protected:
  std::shared_ptr<Logger> logger {nullptr};

private:
  static void trampoline(void* iparam);
  std::unique_ptr<CrossplatformThread> task {nullptr};
};
} // namespace lib7842