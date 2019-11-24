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

TEST(TaskWrapperTest, Task) {
  MockTask task;
  usleep(100000);
  ASSERT_TRUE(task.taskRan);
}