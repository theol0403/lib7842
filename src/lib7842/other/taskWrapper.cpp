#include "taskWrapper.hpp"

namespace lib7842 {

TaskWrapper::TaskWrapper(const std::string& iname) :
  task(std::make_shared<CrossplatformThread>(trampoline, this, iname.c_str())) {}

void TaskWrapper::trampoline(void* iparam) {
  if (!iparam) throw std::runtime_error("TaskWrapper::trampoline: iparam is null");
  static_cast<TaskWrapper*>(iparam)->loop();
}

std::string TaskWrapper::getName() {
  return task->getName();
};

} // namespace lib7842