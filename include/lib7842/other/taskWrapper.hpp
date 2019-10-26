#pragma once
#include "main.h"

namespace lib7842 {
class TaskWrapper {
protected:
  TaskWrapper() = default;
  virtual ~TaskWrapper() = default;

  /**
   * Extend this function to implement custom task.
   */
  virtual void loop() = 0;

  /**
   * Starts the task.
   */
  void startTask(const std::string& iname = "TaskWrapper");

  /**
   * Kills the task.
   */
  void killTask();

  /**
   * Gets the task name.
   */
  std::string getName();

private:
  static void trampoline(void* iparam);
  std::unique_ptr<CrossplatformThread> task {nullptr};
};
} // namespace lib7842