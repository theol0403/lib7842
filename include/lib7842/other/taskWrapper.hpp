#pragma once
#include "main.h"

namespace lib7842 {
class TaskWrapper {
protected:
  TaskWrapper(const std::string& iname = "TaskWrapper");
  TaskWrapper(const TaskWrapper& ipath) = delete;
  virtual ~TaskWrapper() = default;

  /**
   * Extend this function to implement custom task
   */
  virtual void loop() = 0;

  /**
   * Gets the task name.
   */
  std::string getName();

private:
  static void trampoline(void* iparam);
  std::shared_ptr<CrossplatformThread> task {nullptr};
};
} // namespace lib7842