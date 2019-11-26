#define protected public
#include "test.hpp"

class PathFollowerTest : public ::testing::Test {
protected:
  void SetUp() override {
    model = std::make_shared<MockThreeEncoderXDriveModel>();
    odom = std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));

    follower = std::make_shared<PathFollower>(model, odom, 6_in);
  }

  PursuitLimits limits {0_mps, 1_mps, 0.5_mps2, 1};

  std::shared_ptr<MockThreeEncoderXDriveModel> model;
  std::shared_ptr<CustomOdometry> odom;
  std::shared_ptr<PathFollower> follower;
};

TEST_F(PathFollowerTest, TestVelConversions) {
  QSpeed robotVel = 1_mps;
  QAngularSpeed leftWheel = (robotVel / (10_cm)) * 360_deg;
  ASSERT_EQ(600, leftWheel.convert(rpm));
}

TEST_F(PathFollowerTest, TestClosest) {
  PursuitPath path({{0_in, 0_in}, {1_in, 1_in}, {2_in, 2_in}, {3_in, 3_in}, {4_in, 4_in}});
  follower->lastLookIndex = 4;

  auto closest = follower->findClosest(path, {1_in, 1_in});
  ASSERT_EQ(closest - path().begin(), 1);

  closest = follower->findClosest(path, {0_in, 0_in});
  ASSERT_EQ(closest - path().begin(), 1);

  closest = follower->findClosest(path, {3_in, 3.3_in});
  ASSERT_EQ(closest - path().begin(), 3);

  closest = follower->findClosest(path, {6_in, 6_in});
  ASSERT_EQ(closest - path().begin(), 4);

  closest = follower->findClosest(path, {0_in, 0_in});
  ASSERT_EQ(closest - path().begin(), 4);
}