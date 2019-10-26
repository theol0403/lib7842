#include "threeEncXDriveModel.hpp"

namespace lib7842 {

ThreeEncXDriveModel::ThreeEncXDriveModel(
  std::shared_ptr<AbstractMotor> itopLeftMotor,
  std::shared_ptr<AbstractMotor> itopRightMotor,
  std::shared_ptr<AbstractMotor> ibottomRightMotor,
  std::shared_ptr<AbstractMotor> ibottomLeftMotor,
  std::shared_ptr<ContinuousRotarySensor> ileftEnc,
  std::shared_ptr<ContinuousRotarySensor> irightEnc,
  std::shared_ptr<ContinuousRotarySensor> imiddleEnc,
  const double imaxVelocity,
  const double imaxVoltage) :
  XDriveModel(
    std::move(itopLeftMotor),
    std::move(itopRightMotor),
    std::move(ibottomRightMotor),
    std::move(ibottomLeftMotor),
    std::move(ileftEnc),
    std::move(irightEnc),
    imaxVelocity,
    imaxVoltage),
  middleSensor(std::move(imiddleEnc)) {}

std::valarray<std::int32_t> ThreeEncXDriveModel::getSensorVals() const {
  return std::valarray<std::int32_t> {static_cast<std::int32_t>(leftSensor->get()),
                                      static_cast<std::int32_t>(rightSensor->get()),
                                      static_cast<std::int32_t>(middleSensor->get())};
}

void ThreeEncXDriveModel::resetSensors() {
  XDriveModel::resetSensors();
  middleSensor->reset();
}

} // namespace lib7842
