#include "test.hpp"
#include <unistd.h>

class MockTask : public TaskWrapper {
public:
  MockTask() {
    startTask();
  }
  bool taskRan = false;
  void loop() override {
    taskRan = true;
  }
};

SCENARIO("TaskWrapper test") {
  GIVEN("a mock task") {
    MockTask task;
    WHEN("we wait a few milliseconds") {
      usleep(100000);
      THEN("the task should have started") {
        REQUIRE(task.taskRan);
      }
    }
  }
}
