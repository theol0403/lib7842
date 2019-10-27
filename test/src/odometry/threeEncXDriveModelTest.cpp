#include "test.hpp"
#include "lib7842/odometry/threeEncXDriveModel.hpp"

TEST(ThreeEncXDriveModelTest, Funcionality) {

  auto topLeftMotor = std::make_shared<MockMotor>();
  auto topRightMotor = std::make_shared<MockMotor>();
  auto bottomRightMotor = std::make_shared<MockMotor>();
  auto bottomLeftMotor = std::make_shared<MockMotor>();

  auto leftEnc = std::make_shared<MockContinuousRotarySensor>();
  leftEnc->value = 10;

  auto rightEnc = std::make_shared<MockContinuousRotarySensor>();
  rightEnc->value = 15;

  auto middleEnc = std::make_shared<MockContinuousRotarySensor>();
  middleEnc->value = 200;

  ThreeEncXDriveModel model(
    topLeftMotor, topRightMotor, bottomRightMotor, bottomLeftMotor, leftEnc, rightEnc, middleEnc);

  auto sensors = model.getSensorVals();
  ASSERT_EQ(sensors[0], 10);
  ASSERT_EQ(sensors[1], 15);
  ASSERT_EQ(sensors[2], 200);

  model.resetSensors();
  auto sensors2 = model.getSensorVals();
  ASSERT_EQ(sensors2[0], 0);
  ASSERT_EQ(sensors2[1], 0);
  ASSERT_EQ(sensors2[2], 0);
}