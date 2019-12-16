#define protected public
#include "test.hpp"

TEST_CASE("PathFollower test") {

  SUBCASE("given a model, odom, follower, and limits") {

    auto model = std::make_shared<MockThreeEncoderXDriveModel>();
    auto odom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}), createTimeUtil());

    auto follower = std::make_shared<PathFollower>(model, odom, ChassisScales({{4_in, 10_in}, 360}),
                                                   6_in, createTimeUtil());

    PursuitLimits limits {0_mps, 0.5_mps2, 1_mps, 1_mps};

    SUBCASE("TestClosest") {
      PursuitPath path({{0_ft, 0_ft}, {1_ft, 1_ft}, {2_ft, 2_ft}, {3_ft, 3_ft}, {4_ft, 4_ft}});
      follower->lastLookIndex = 4;

      auto closest = follower->findClosest(path, {1_ft, 1_ft});
      CHECK(closest - path().begin() == 1);

      closest = follower->findClosest(path, {0_ft, 0_ft});
      CHECK(closest - path().begin() == 1);

      closest = follower->findClosest(path, {3_ft, 3.3_ft});
      CHECK(closest - path().begin() == 3);

      closest = follower->findClosest(path, {6_ft, 6_ft});
      CHECK(closest - path().begin() == 4);

      closest = follower->findClosest(path, {0_ft, 0_ft});
      CHECK(closest - path().begin() == 4);
    }

    SUBCASE("TestLookahead") {
      PursuitPath path({{0_ft, 0_ft}, {0_ft, 1_ft}, {0_ft, 2_ft}, {0_ft, 3_ft}, {0_ft, 4_ft}});

      Vector lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
      Vector estimated {0_ft, 1.5_ft};
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
      estimated = {0_ft, 2.5_ft};
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 0_ft});
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 3.5_ft});
      estimated = {0_ft, 4_ft};
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));

      lookahead = follower->findLookaheadPoint(path, {0_ft, 4_ft});
      estimated = {0_ft, 4_ft};
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));

      lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));
    }

    SUBCASE("TestCurvature") {
      auto curvature = PathFollower::calculateCurvature({0_in, 0_in, 0_deg}, {0_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {5_in, 0_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {-5_in, 0_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {5_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {-5_in, -5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {10_in, 5_in});
      CHECK(std::abs(curvature) > 2);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, -45_deg}, {-5_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = PathFollower::calculateCurvature({0_in, 0_in, 200_deg}, {10_in, 5_in});
      CHECK(std::abs(curvature) > 4);
    }

    SUBCASE("TestVelocityStraight") {
      auto vel = PathFollower::calculateVelocity(1_mps, 0, ChassisScales({{1_m / 1_pi, 10_m}, 360}),
                                                 {0_mps, 1_mps2, 10_mps, 1_mps});
      CHECK(vel[0] == 60_rpm);
      CHECK(vel[1] == 60_rpm);
    }

    SUBCASE("TestVelocityCurved") {
      auto vel = PathFollower::calculateVelocity(1_mps, 1, ChassisScales({{1_m / 1_pi, 10_m}, 360}),
                                                 {0_mps, 1_mps2, 10_mps, 1_mps});
      CHECK(vel[0] > 60_rpm);
      CHECK(vel[1] < 60_rpm);
    }
  }

  SUBCASE("TestVelConversions") {
    QSpeed robotVel = 1_mps;
    QAngularSpeed leftWheel = (robotVel / 10_cm) * 360_deg;
    CHECK(leftWheel == 600_rpm);
  }

  SUBCASE("ReverseVelConversions") {
    QAngularSpeed wheel = (1_mps / (1_pi * 10_cm)) * 360_deg;
    QSpeed vel = (wheel * 1_pi * 10_cm) / 360_deg;
    CHECK(vel == 1_mps);
  }
}