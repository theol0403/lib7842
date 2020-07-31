#include "pros/rtos.hpp"
#include <unistd.h>

#define protected public
#include "../test/include/test.hpp"

extern "C" {
namespace pros::c {

void delay(const uint32_t milliseconds) {
  usleep(1000 * milliseconds);
}

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

} // namespace pros::c

namespace okapi {

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

} // namespace okapi

namespace lib7842 {
std::ostream& operator<<(std::ostream& os, const Vector& rhs) {
  os << "{" << rhs.x << ", " << rhs.y << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const State& rhs) {
  os << "{" << rhs.x << ", " << rhs.y << ", " << rhs.theta << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const DataPoint& rhs) {
  os << "{" << rhs.x << ", " << rhs.y;
  bool first = true;
  for (auto&& data : rhs.data) {
    if (first) {
      os << ", [";
      first = false;
    } else {
      os << ", ";
    }
    os << data.first;
  }
  if (!first) { os << "]"; }
  os << "}";
  return os;
}
} // namespace lib7842
