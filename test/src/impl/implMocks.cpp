#include "implMocks.hpp"
#include "okapi/api/chassis/model/threeEncoderXDriveModel.hpp"
#include "pros/rtos.hpp"
#include <unistd.h>

#define protected public
#include "lib7842/test.hpp"
using namespace test;

extern "C" {
namespace pros::c {

void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

} // namespace pros::c
} // extern "C"

namespace test {

class MockThreeEncoderXDriveModel : public ThreeEncoderXDriveModel {
public:
  MockThreeEncoderXDriveModel();
  std::valarray<std::int32_t> getSensorVals() const override;
  void setSensorVals(std::int32_t left, std::int32_t right, std::int32_t middle);

  std::int32_t leftEnc {0};
  std::int32_t rightEnc {0};
  std::int32_t middleEnc {0};
};

MockThreeEncoderXDriveModel::MockThreeEncoderXDriveModel() :
  ThreeEncoderXDriveModel(std::make_shared<MockMotor>(), std::make_shared<MockMotor>(),
                          std::make_shared<MockMotor>(), std::make_shared<MockMotor>(),
                          std::make_shared<MockContinuousRotarySensor>(),
                          std::make_shared<MockContinuousRotarySensor>(),
                          std::make_shared<MockContinuousRotarySensor>(), 200, 12000) {}

std::valarray<std::int32_t> MockThreeEncoderXDriveModel::getSensorVals() const {
  return std::valarray<std::int32_t> {leftEnc, rightEnc, middleEnc};
}

void MockThreeEncoderXDriveModel::setSensorVals(std::int32_t left, std::int32_t right,
                                                std::int32_t middle) {
  leftEnc = left;
  rightEnc = right;
  middleEnc = middle;
}

} // namespace test
