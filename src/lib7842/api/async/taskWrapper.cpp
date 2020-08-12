#include "lib7842/api/async/taskWrapper.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

void TaskWrapper::loop() { GLOBAL_ERROR_THROW("TaskWrapper::loop: loop is not overridden"); }

void TaskWrapper::startTask(const std::string& iname) {
  if (task) GLOBAL_INFO("TaskWrapper::startTask: restarting task: " + iname);
  task = std::make_unique<CrossplatformThread>(trampoline, this, iname.c_str());
}

void TaskWrapper::stopTask() { task = nullptr; }

std::string TaskWrapper::getName() { return task->getName(); };

void TaskWrapper::trampoline(void* iparam) { static_cast<TaskWrapper*>(iparam)->loop(); }

} // namespace lib7842

// #include "lib7842/test/test.hpp"
// namespace test {
// class MockTask : public TaskWrapper {
// public:
//   MockTask() { startTask(); }
//   bool taskRan = false;
//   void loop() override { taskRan = true; }
// };

// TEST_CASE("TaskWrapper") {
//   GIVEN("a mock task") {
//     MockTask task;
//     WHEN("we wait a few milliseconds") {
//       pros::delay(100);
//       THEN("the task should have started") { REQUIRE(task.taskRan); }
//     }
//   }
// }
// } // namespace test
