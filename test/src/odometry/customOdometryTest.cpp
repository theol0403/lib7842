#include "test.hpp"
#include "lib7842/odometry/customOdometry.hpp"

class MockThreeEncXDriveModel : public ThreeEncXDriveModel {
public:
  MockThreeEncXDriveModel() :
    ThreeEncXDriveModel(
      std::make_shared<MockMotor>(),
      std::make_shared<MockMotor>(),
      std::make_shared<MockMotor>(),
      std::make_shared<MockMotor>(),
      std::make_shared<MockContinuousRotarySensor>(),
      std::make_shared<MockContinuousRotarySensor>(),
      std::make_shared<MockContinuousRotarySensor>()) {}

  std::valarray<std::int32_t> getSensorVals() const override {
    return std::valarray<std::int32_t> {leftEnc, rightEnc, middleEnc};
  }

  void setSensorVals(std::int32_t left, std::int32_t right, std::int32_t middle) {
    leftEnc = left;
    rightEnc = right;
    middleEnc = middle;
  }

  std::int32_t leftEnc {0};
  std::int32_t rightEnc {0};
  std::int32_t middleEnc {0};
};

class CustomOdometryTest : public ::testing::Test {
protected:
  void SetUp() override {
    model = std::make_shared<MockThreeEncXDriveModel>();
    odom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 10_in, 4_in}, 360}));
  }

  QLength calculateDistanceTraveled(int ticks) {
    return (ticks / 360.0) * 1_pi * 4_in;
  }

  void assertOdomState(const State& istate, std::shared_ptr<CustomOdometry> iodom = nullptr) {
    if (!iodom) iodom = odom;
    const auto error = 1e-4;
    EXPECT_NEAR(iodom->getState().x.convert(meter), istate.x.convert(meter), error);
    EXPECT_NEAR(iodom->getState().y.convert(meter), istate.y.convert(meter), error);
    EXPECT_NEAR(iodom->getState().theta.convert(degree), istate.theta.convert(degree), error);
  }

  std::shared_ptr<MockThreeEncXDriveModel> model;
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

// TEST_F(CustomOdometryTest, NoSensorMovementDoesNotAffectState) {
//   assertOdomState({0_m, 0_m, 0_deg});
//   odom->step();
//   assertOdomState({0_m, 0_m, 0_deg});
// }

// TEST_F(CustomOdometryTest, MoveForwardTest) {
//   model->setSensorVals(10, 10, 0);
//   odom->step();
//   assertOdomState({0_m, calculateDistanceTraveled(10), 0_deg});

//   model->setSensorVals(20, 20, 0);
//   odom->step();
//   assertOdomState({0_m, calculateDistanceTraveled(20), 0_deg});

//   model->setSensorVals(10, 10, 0);
//   odom->step();
//   assertOdomState({0_m, calculateDistanceTraveled(10), 0_deg});
// }

TEST_F(CustomOdometryTest, TurnInPlaceTest) {
  model->setSensorVals(10, -10, -10);
  odom->step();
  assertOdomState({0_m, 0_m, 4_deg});

  // model->setSensorVals(0, 0, 0);
  // odom->step();
  // assertOdomState({0_m, 0_m, 0_deg});

  // model->setSensorVals(-10, 10, 10);
  // odom->step();
  // assertOdomState({0_m, 0_m, -4_deg});
}

// TEST_F(CustomOdometryTest, TurnAndDriveTest) {
//   model->setSensorVals(90, -90, -90);
//   odom->step();
//   assertOdomState({0_m, 0_m, 36_deg});

//   model->setSensorVals(180, 0, -90);
//   odom->step();
//   assertOdomState({//                         calculateDistanceTraveled(90) * std::cos((36_deg).convert(radian)}),
//                         calculateDistanceTraveled(90) * std::sin((36_deg).convert(radian)),
//                         36_deg);
// }

// TEST_F(CustomOdometryTest, StrafeTest) {
//   model->setSensorVals(0, 0, 10);
//   odom->step();
//   assertOdomState({0_in, calculateDistanceTraveled(10), 0_deg});
// }

// TEST_F(CustomOdometryTest, DriveForwardWhileStrafingTest) {
//   model->setSensorVals(10, 10, 10);
//   odom->step();
//   assertOdomState({calculateDistanceTraveled(10), calculateDistanceTraveled(10), 0_deg});
// }

// TEST_F(CustomOdometryTest, SmallSwingTurnOnRightWheels) {
//   auto model = std::make_shared<MockThreeEncXDriveModel>();
//   ThreeEncoderOdometry odom(createConstantTimeUtil(10_ms),
//                             model,
//                             ChassisScales{{2.75_in, 12.9_in, 1_in, 2.75_in}, quadEncoderTPR});

//   model->setSensorVals(0, 0, 0);
//   odom.step();
//   assertOdomState(&odom, 0_in, 0_in, 0_deg);

//   model->setSensorVals(0, -2, 0);
//   odom.step();
//   assertOdomState(&odom, -0.024_in, 0_in, 0.2131_deg);
// }
