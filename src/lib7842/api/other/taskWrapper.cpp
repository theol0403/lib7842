#include "taskWrapper.hpp"

namespace lib7842 {

void TaskWrapper::loop() {
  auto logger = global::getLogger();
  std::string msg("TaskWrapper::loop: loop is not overridden");
  LOG_ERROR(msg);
  throw std::runtime_error(msg);
}

void TaskWrapper::startTask(const std::string& iname) {
  auto logger = global::getLogger();
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