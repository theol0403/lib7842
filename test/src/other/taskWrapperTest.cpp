#include "test.hpp"
#include "lib7842/other/taskWrapper.hpp"
#include <unistd.h>

class MockTask : public TaskWrapper {
public:
  MockTask() {
    startTask();
  }
  bool taskRan = false;
  virtual void loop() override {
    taskRan = true;
  }
};

TEST(TaskWrapperTest, Task) {
  MockTask task;
  usleep(1000);
  ASSERT_TRUE(task.taskRan);
}

class MockEndlessTask : public EndlessTaskWrapper {
public:
  MockEndlessTask() {}
  bool taskRan = false;
  virtual void loop() override {
    taskRan = true;
  }
};

TEST(TaskWrapperTest, EndlessTask) {
  MockEndlessTask task;
  usleep(1000);
  ASSERT_TRUE(task.taskRan);
}