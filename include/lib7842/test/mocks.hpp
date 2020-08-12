#include "lib7842/test/test.hpp"
#include "okapi/api/chassis/model/threeEncoderXDriveModel.hpp"
#include "okapi/api/control/util/settledUtil.hpp"
#include "okapi/api/util/timeUtil.hpp"

namespace test {
std::unique_ptr<SettledUtil> createSettledUtilPtr(double iatTargetError = 50,
                                                  double iatTargetDerivative = 5,
                                                  QTime iatTargetTime = 250_ms);

TimeUtil createTimeUtil();
TimeUtil createConstantTimeUtil(QTime idt);
TimeUtil createTimeUtil(const Supplier<std::unique_ptr<AbstractTimer>>& itimerSupplier);
TimeUtil createTimeUtil(const Supplier<std::unique_ptr<SettledUtil>>& isettledUtilSupplier);

class MockTimer : public AbstractTimer {
public:
  MockTimer();
  QTime millis() const override;
};

class MockThreeEncoderXDriveModel : public ThreeEncoderXDriveModel {
public:
  MockThreeEncoderXDriveModel();
  std::valarray<std::int32_t> getSensorVals() const override;
  void setSensorVals(std::int32_t left, std::int32_t right, std::int32_t middle);

  std::int32_t leftEnc {0};
  std::int32_t rightEnc {0};
  std::int32_t middleEnc {0};
};

class MockContinuousRotarySensor : public ContinuousRotarySensor {
public:
  double controllerGet() override;
  int32_t reset() override;
  double get() const override;
  mutable std::int32_t value {0};
};

class MockMotor : public AbstractMotor {
public:
  MockMotor();
  void controllerSet(double ivalue) override;
  int32_t moveAbsolute(double iposition, std::int32_t ivelocity) override;
  int32_t moveRelative(double iposition, std::int32_t ivelocity) override;
  double getTargetPosition() override;
  double getPosition() override;
  int32_t getTargetVelocity() override;
  double getActualVelocity() override;
  int32_t tarePosition() override;
  int32_t setBrakeMode(brakeMode imode) override;
  int32_t setCurrentLimit(std::int32_t ilimit) override;
  int32_t setEncoderUnits(encoderUnits iunits) override;
  int32_t setGearing(gearset igearset) override;
  int32_t setReversed(bool ireverse) override;
  int32_t setVoltageLimit(std::int32_t ilimit) override;
  std::shared_ptr<ContinuousRotarySensor> getEncoder() override;
  virtual std::shared_ptr<MockContinuousRotarySensor> getMockEncoder();
  std::int32_t moveVelocity(std::int16_t ivelocity) override;
  std::int32_t moveVoltage(std::int16_t ivoltage) override;
  int32_t getCurrentDraw() override;
  int32_t getDirection() override;
  double getEfficiency() override;
  int32_t isOverCurrent() override;
  int32_t isOverTemp() override;
  int32_t isStopped() override;
  int32_t getZeroPositionFlag() override;
  uint32_t getFaults() override;
  uint32_t getFlags() override;
  int32_t getRawPosition(std::uint32_t* timestamp) override;
  double getPower() override;
  double getTemperature() override;
  double getTorque() override;
  int32_t getVoltage() override;
  int32_t modifyProfiledVelocity(std::int32_t ivelocity) override;
  AbstractMotor::brakeMode getBrakeMode() override;
  int32_t getCurrentLimit() override;
  AbstractMotor::encoderUnits getEncoderUnits() override;
  AbstractMotor::gearset getGearing() override;
  std::shared_ptr<MockContinuousRotarySensor> encoder;
  mutable std::int16_t lastVelocity {0};
  mutable std::int16_t maxVelocity {0};
  mutable std::int16_t lastVoltage {0};
  mutable std::int16_t lastPosition {0};
  mutable std::int32_t lastProfiledMaxVelocity {0};
  AbstractMotor::gearset gearset {AbstractMotor::gearset::green};
  AbstractMotor::encoderUnits encoderUnits {AbstractMotor::encoderUnits::counts};
  AbstractMotor::brakeMode brakeMode {AbstractMotor::brakeMode::coast};
};
} // namespace test
