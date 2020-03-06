#include "taskWrapper.hpp"

namespace lib7842 {

TaskWrapper::TaskWrapper(const std::shared_ptr<Logger>& ilogger) : logger(ilogger) {}

void TaskWrapper::loop() {
  std::string msg("TaskWrapper::loop: loop is not overridden");
  LOG_ERROR(msg);
  throw std::runtime_error(msg);
}

void TaskWrapper::startTask(const std::string& iname) {
  if (task) LOG_INFO("TaskWrapper::startTask: restarting task: " + iname);
  task = std::make_unique<CrossplatformThread>(trampoline, this, iname.c_str());
}

void TaskWrapper::stopTask() {
  task = nullptr;
}

std::string TaskWrapper::getName() {
  return task->getName();
};

void TaskWrapper::trampoline(void* iparam) {
  pros::delay(20);
  static_cast<TaskWrapper*>(iparam)->loop();
}

} // namespace lib7842