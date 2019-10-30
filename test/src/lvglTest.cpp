#include "test.hpp"
#include "main.h"

#include "lib7842/gui/odomDebug.hpp"
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
    return std::valarray<std::int32_t> {leftEnc, rightEnc};
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

void lvglTest() {

  // lv_obj_t* button = lv_btn_create(lv_scr_act(), NULL);

  auto model = std::make_shared<MockThreeEncXDriveModel>();
  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));

  OdomDebug debug(lv_scr_act(), LV_COLOR_HEX(0xFF7F00));
  debug.initialize();

  debug.attachOdom(odom);
  // debug.attachResetter([&]() {
  //   odom->reset();
  // });

  while (true) {
    debug.render();
    pros::delay(100);
  }
}