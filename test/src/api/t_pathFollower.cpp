#include "test.hpp"

class PathFollowerTest : public ::testing::Test {
protected:
};

TEST_F(PathFollowerTest, TestVelConversions) {
  QSpeed robotVel = 1_mps;
  QAngularSpeed leftWheel = (robotVel / (10_cm)) * 360_deg;
  ASSERT_EQ(600, leftWheel.convert(rpm));
}