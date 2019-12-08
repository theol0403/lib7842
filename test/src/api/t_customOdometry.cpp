#include "test.hpp"

class CustomOdometryTest : public ::testing::Test {
protected:
  void SetUp() override {
    model = std::make_shared<MockThreeEncoderXDriveModel>();
    odom = std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}),
                                            createTimeUtil());
  }

  QLength calculateDistanceTraveled(int ticks) {
    return (ticks / 360.0) * 1_pi * 4_in;
  }

  void assertOdomState(const State& istate, std::shared_ptr<CustomOdometry> iodom = nullptr,
                       double error = 1e-10) {
    if (!iodom) iodom = odom;
    EXPECT_NEAR(iodom->getState().x.convert(meter), istate.x.convert(meter), error);
    EXPECT_NEAR(iodom->getState().y.convert(meter), istate.y.convert(meter), error);
    EXPECT_NEAR(iodom->getState().theta.convert(degree), istate.theta.convert(degree), error);
  }

  std::shared_ptr<MockThreeEncoderXDriveModel> model;
  std::shared_ptr<CustomOdometry> odom;
};

TEST_F(CustomOdometryTest, MockSensorsFunction) {
  std::shared_ptr<ChassisModel> imodel = model;
  model->setSensorVals(10, 20, 30);
  auto sensors = imodel->getSensorVals();
  ASSERT_EQ(sensors[0], 10);
  ASSERT_EQ(sensors[1], 20);
  ASSERT_EQ(sensors[2], 30);
}

TEST_F(CustomOdometryTest, NoSensorMovementDoesNotAffectState) {
  assertOdomState({0_m, 0_m, 0_deg});
  odom->step();
  assertOdomState({0_m, 0_m, 0_deg});
}

TEST_F(CustomOdometryTest, RepeatedlyCallingStep) {
  for (size_t i = 0; i < 1000; i++) {
    assertOdomState({0_m, 0_m, 0_deg}, odom, 1e-20);
    odom->step();
    assertOdomState({0_m, 0_m, 0_deg}, odom, 1e-20);
  }
}

TEST_F(CustomOdometryTest, MoveForwardTest) {
  model->setSensorVals(10, 10, 0);
  odom->step();
  assertOdomState({0_m, calculateDistanceTraveled(10), 0_deg});

  model->setSensorVals(20, 20, 0);
  odom->step();
  assertOdomState({0_m, calculateDistanceTraveled(20), 0_deg});

  model->setSensorVals(10, 10, 0);
  odom->step();
  assertOdomState({0_m, calculateDistanceTraveled(10), 0_deg});
}

TEST_F(CustomOdometryTest, TurnInPlaceTest) {
  model->setSensorVals(10, -10, -10);
  odom->step();
  assertOdomState({0_m, 0_m, 4_deg});

  model->setSensorVals(0, 0, 0);
  odom->step();
  assertOdomState({0_m, 0_m, 0_deg});

  model->setSensorVals(-10, 10, 10);
  odom->step();
  assertOdomState({0_m, 0_m, -4_deg});
}

TEST_F(CustomOdometryTest, TurnAndDriveTest) {
  model->setSensorVals(90, -90, -90);
  odom->step();
  assertOdomState({0_m, 0_m, 36_deg});

  model->setSensorVals(180, 0, -90);
  odom->step();
  assertOdomState({calculateDistanceTraveled(90) * std::sin((36_deg).convert(radian)),
                   calculateDistanceTraveled(90) * std::cos((36_deg).convert(radian)), 36_deg});
}

TEST_F(CustomOdometryTest, StrafeTest) {
  model->setSensorVals(0, 0, 10);
  odom->step();
  assertOdomState({calculateDistanceTraveled(10), 0_in, 0_deg});
}

TEST_F(CustomOdometryTest, DriveForwardWhileStrafingTest) {
  model->setSensorVals(10, 10, 10);
  odom->step();
  assertOdomState({calculateDistanceTraveled(10), calculateDistanceTraveled(10), 0_deg});
}

TEST_F(CustomOdometryTest, SmallSwingTurnOnRightWheels) {
  auto smallOdom = std::make_shared<CustomOdometry>(
    model, ChassisScales({{2.75_in, 12.9_in, 0.5_in, 2.75_in}, quadEncoderTPR}), createTimeUtil());

  model->setSensorVals(0, 0, 0);
  smallOdom->step();
  assertOdomState({0_in, 0_in, 0_deg}, smallOdom);

  model->setSensorVals(0, -2, 0);
  smallOdom->step();
  assertOdomState({0_in, -0.024_in, 0.2131_deg}, smallOdom, 1e-4);
}

TEST_F(CustomOdometryTest, NinetyDegreePivotTurn) {
  auto straightOdom = std::make_shared<CustomOdometry>(
    model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

  assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

  model->setSensorVals(450, 0, 0);
  straightOdom->step();
  assertOdomState({5_in, 5_in, 90_deg}, straightOdom);
}

TEST_F(CustomOdometryTest, FullPivotRotation) {
  auto straightOdom = std::make_shared<CustomOdometry>(
    model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

  assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

  model->setSensorVals(1800, 0, 0);
  straightOdom->step();
  assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
}

TEST_F(CustomOdometryTest, FullPointRotation) {
  auto straightOdom = std::make_shared<CustomOdometry>(
    model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

  assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

  model->setSensorVals(900, -900, 0);
  straightOdom->step();
  assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
}

TEST_F(CustomOdometryTest, FullPointRotationWithMiddle) {
  assertOdomState({0_m, 0_m, 0_deg});

  model->setSensorVals(900, -900, -900);
  odom->step();
  assertOdomState({0_in, 0_in, 360_deg});
}
