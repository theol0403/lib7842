#pragma once
#include "main.h"

#include "okapi/api/chassis/model/xDriveModel.hpp"

namespace lib7842 {

class ThreeEncXDriveModel : public XDriveModel {
public:
  ThreeEncXDriveModel(
    std::shared_ptr<AbstractMotor> itopLeftMotor,
    std::shared_ptr<AbstractMotor> itopRightMotor,
    std::shared_ptr<AbstractMotor> ibottomRightMotor,
    std::shared_ptr<AbstractMotor> ibottomLeftMotor,
    std::shared_ptr<ContinuousRotarySensor> ileftEnc,
    std::shared_ptr<ContinuousRotarySensor> irightEnc,
    std::shared_ptr<ContinuousRotarySensor> imiddleEnc,
    double imaxVelocity,
    double imaxVoltage);

  std::valarray<std::int32_t> getSensorVals() const override;

  void resetSensors() override;

protected:
  std::shared_ptr<ContinuousRotarySensor> middleSensor;
};

} // namespace lib7842