#pragma once
#include "main.h"

#include "okapi/api/chassis/model/xDriveModel.hpp"

namespace lib7842 {

class ThreeEncXDriveModel : public XDriveModel {
public:
  ThreeEncXDriveModel(
    const std::shared_ptr<AbstractMotor>& itopLeftMotor,
    const std::shared_ptr<AbstractMotor>& itopRightMotor,
    const std::shared_ptr<AbstractMotor>& ibottomRightMotor,
    const std::shared_ptr<AbstractMotor>& ibottomLeftMotor,
    const std::shared_ptr<ContinuousRotarySensor>& ileftEnc,
    const std::shared_ptr<ContinuousRotarySensor>& irightEnc,
    const std::shared_ptr<ContinuousRotarySensor>& imiddleEnc,
    double imaxVelocity = 200,
    double imaxVoltage = 12000);

  std::valarray<std::int32_t> getSensorVals() const override;

  void resetSensors() override;

protected:
  std::shared_ptr<ContinuousRotarySensor> middleSensor;
};

} // namespace lib7842