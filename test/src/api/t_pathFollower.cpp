#define protected public
#include "test.hpp"

class PathFollowerTest : public ::testing::Test {
protected:
  void SetUp() override {
    model = std::make_shared<MockThreeEncoderXDriveModel>();
    odom = std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}),
                                            createTimeUtil());

    follower = std::make_shared<PathFollower>(model, odom, 10_in, 6_in, createTimeUtil());
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

TEST_F(PathFollowerTest, ReverseVelConversions) {
  QAngularSpeed wheel = (1_mps / (1_pi * 10_cm)) * 360_deg;
  QSpeed vel = (wheel * 1_pi * 10_cm) / 360_deg;
  ASSERT_EQ(vel.convert(mps), 1);
}

TEST_F(PathFollowerTest, TestClosest) {
  PursuitPath path({{0_ft, 0_ft}, {1_ft, 1_ft}, {2_ft, 2_ft}, {3_ft, 3_ft}, {4_ft, 4_ft}});
  follower->lastLookIndex = 4;

  auto closest = follower->findClosest(path, {1_ft, 1_ft});
  ASSERT_EQ(closest - path().begin(), 1);

  closest = follower->findClosest(path, {0_ft, 0_ft});
  ASSERT_EQ(closest - path().begin(), 1);

  closest = follower->findClosest(path, {3_ft, 3.3_ft});
  ASSERT_EQ(closest - path().begin(), 3);

  closest = follower->findClosest(path, {6_ft, 6_ft});
  ASSERT_EQ(closest - path().begin(), 4);

  closest = follower->findClosest(path, {0_ft, 0_ft});
  ASSERT_EQ(closest - path().begin(), 4);
}

TEST_F(PathFollowerTest, TestLookahead) {
  PursuitPath path({{0_ft, 0_ft}, {0_ft, 1_ft}, {0_ft, 2_ft}, {0_ft, 3_ft}, {0_ft, 4_ft}});

  Vector lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
  Vector estimated {0_ft, 1.5_ft};
  ASSERT_EQ(lookahead, estimated);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
  ASSERT_EQ(lookahead, estimated);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
  estimated = {0_ft, 2.5_ft};
  ASSERT_EQ(lookahead, estimated);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 0_ft});
  ASSERT_EQ(lookahead, estimated);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 3.5_ft});
  estimated = {0_ft, 4_ft};
  ASSERT_NEAR(lookahead.y.convert(foot), estimated.y.convert(foot), 1e-10);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 4_ft});
  estimated = {0_ft, 4_ft};
  ASSERT_NEAR(lookahead.y.convert(foot), estimated.y.convert(foot), 1e-10);

  lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
  ASSERT_NEAR(lookahead.y.convert(foot), estimated.y.convert(foot), 1e-10);
}

TEST_F(PathFollowerTest, TestCurvature) {
  auto curvature = PathFollower::calculateCurvature({0_in, 0_in, 0_deg}, {0_in, 5_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {5_in, 0_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {-5_in, 0_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {5_in, 5_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {-5_in, -5_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {10_in, 5_in});
  ASSERT_GT(std::abs(curvature), 2);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, -45_deg}, {-5_in, 5_in});
  ASSERT_LT(std::abs(curvature), 1e-4);

  curvature = PathFollower::calculateCurvature({0_in, 0_in, 200_deg}, {10_in, 5_in});
  ASSERT_GT(std::abs(curvature), 4);
}