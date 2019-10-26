#include "taskWrapper.hpp"

namespace lib7842 {

void TaskWrapper::startTask(const std::string& iname) {
  if (!task) std::cerr << "Warning: restarting task: " << iname << std::endl;
  task = std::make_shared<CrossplatformThread>(trampoline, this, iname.c_str());
}

void TaskWrapper::killTask() {
  task = nullptr;
}

std::string TaskWrapper::getName() {
  return task->getName();
};

void TaskWrapper::trampoline(void* iparam) {
  if (!iparam) throw std::runtime_error("TaskWrapper::trampoline: iparam is null");
  static_cast<TaskWrapper*>(iparam)->loop();
}

} // namespace lib7842