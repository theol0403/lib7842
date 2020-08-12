#include "lib7842/test/mocks.hpp"

namespace test {

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
