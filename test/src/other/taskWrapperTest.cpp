#include "test.hpp"
#include "lib7842/other/taskWrapper.hpp"
#include <unistd.h>

class MockTask : public TaskWrapper {
public:
  bool taskRan = false;
  virtual void loop() {
    taskRan = true;
  }
};

TEST(TaskWrapperTest, Functionality) {
  MockTask task;
  usleep(100);
  ASSERT_TRUE(task.taskRan);
}