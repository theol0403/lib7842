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
  virtual void startTask(const std::string& iname = "TaskWrapper");

  /**
   * Kills the task.
   */
  virtual void killTask();

  /**
   * Gets the task name.
   */
  virtual std::string getName();

private:
  static void trampoline(void* iparam);
  std::unique_ptr<CrossplatformThread> task {nullptr};
};

class EndlessTaskWrapper : public TaskWrapper {
public:
  /**
   * Automatically starts the task
   */
  explicit EndlessTaskWrapper(const std::string& iname = "TaskWrapper");

private:
  using TaskWrapper::startTask;
  using TaskWrapper::killTask;
};
} // namespace lib7842