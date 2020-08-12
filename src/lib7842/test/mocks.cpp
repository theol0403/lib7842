#include "lib7842/test/mocks.hpp"
#include "okapi/api/units/QTime.hpp"
#include "pros/rtos.hpp"

namespace test {

MockTimer::MockTimer() : AbstractTimer(millis()) {}
#ifndef THREADS_STD
QTime MockTimer::millis() const { return pros::millis() * millisecond; }
#else
std::chrono::system_clock::time_point epoch = std::chrono::high_resolution_clock::from_time_t(0);

QTime MockTimer::millis() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
           std::chrono::high_resolution_clock::now() - epoch)
           .count() *
         millisecond;
}
#endif

ConstantMockTimer::ConstantMockTimer(const QTime idt) : AbstractTimer(0_ms), dtToReturn(idt) {}
QTime ConstantMockTimer::millis() const { return 0_ms; }
QTime ConstantMockTimer::getDt() { return dtToReturn; }
QTime ConstantMockTimer::readDt() const { return dtToReturn; }
QTime ConstantMockTimer::getStartingTime() const { return 0_ms; }
QTime ConstantMockTimer::getDtFromStart() const { return dtToReturn; }
void ConstantMockTimer::placeMark() {}
void ConstantMockTimer::placeHardMark() {}
QTime ConstantMockTimer::clearHardMark() { return 0_ms; }
QTime ConstantMockTimer::getDtFromMark() const { return dtToReturn; }
QTime ConstantMockTimer::getDtFromHardMark() const { return dtToReturn; }
bool ConstantMockTimer::repeat(QTime) { return false; }
bool ConstantMockTimer::repeat(QFrequency) { return false; }
QTime ConstantMockTimer::clearMark() { return 0_ms; }

MockRate::MockRate() = default;
#ifndef THREADS_STD
void MockRate::delay(QFrequency ihz) { pros::delay(1000 / static_cast<int>(ihz.convert(Hz))); }
void MockRate::delayUntil(uint32_t ims) { pros::delay(ims); }
#else
void MockRate::delay(QFrequency ihz) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000 / static_cast<int>(ihz.convert(Hz))));
}

void MockRate::delayUntil(uint32_t ims) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ims));
}
#endif
void MockRate::delayUntil(QTime itime) {
  delayUntil(static_cast<uint32_t>(itime.convert(millisecond)));
}

std::unique_ptr<SettledUtil> createSettledUtilPtr(const double iatTargetError,
                                                  const double iatTargetDerivative,
                                                  const QTime iatTargetTime) {
  return std::make_unique<SettledUtil>(std::make_unique<MockTimer>(), iatTargetError,
                                       iatTargetDerivative, iatTargetTime);
}

TimeUtil createTimeUtil() {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>([]() { return std::make_unique<MockTimer>(); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createConstantTimeUtil(const QTime idt) {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>(
      [=]() { return std::make_unique<ConstantMockTimer>(idt); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createTimeUtil(const Supplier<std::unique_ptr<AbstractTimer>>& itimerSupplier) {
  return TimeUtil(itimerSupplier, Supplier<std::unique_ptr<AbstractRate>>([]() {
                    return std::make_unique<MockRate>();
                  }),
                  Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(); }));
}

TimeUtil createTimeUtil(const Supplier<std::unique_ptr<SettledUtil>>& isettledUtilSupplier) {
  return TimeUtil(
    Supplier<std::unique_ptr<AbstractTimer>>([]() { return std::make_unique<MockTimer>(); }),
    Supplier<std::unique_ptr<AbstractRate>>([]() { return std::make_unique<MockRate>(); }),
    isettledUtilSupplier);
}

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
double MockContinuousRotarySensor::controllerGet() { return value; }
int32_t MockContinuousRotarySensor::reset() {
  value = 0;
  return 0;
}
double MockContinuousRotarySensor::get() const { return value; }

MockMotor::MockMotor() : encoder(std::make_shared<MockContinuousRotarySensor>()) {}
void MockMotor::controllerSet(const double ivalue) { moveVelocity((int16_t)ivalue); }
int32_t MockMotor::moveAbsolute(const double iposition, const std::int32_t ivelocity) {
  lastPosition = (int16_t)iposition;
  lastProfiledMaxVelocity = ivelocity;
  return 0;
}
int32_t MockMotor::moveRelative(const double iposition, const std::int32_t ivelocity) {
  lastPosition += static_cast<std::int16_t>(iposition);
  lastProfiledMaxVelocity = ivelocity;
  return 0;
}
double MockMotor::getTargetPosition() { return 0; }
double MockMotor::getPosition() { return encoder->get(); }
int32_t MockMotor::getTargetVelocity() { return 0; }
double MockMotor::getActualVelocity() { return 0; }
int32_t MockMotor::tarePosition() { return 0; }
int32_t MockMotor::setBrakeMode(const AbstractMotor::brakeMode imode) {
  brakeMode = imode;
  return 0;
}
int32_t MockMotor::setCurrentLimit(const std::int32_t) { return 0; }
int32_t MockMotor::setEncoderUnits(const AbstractMotor::encoderUnits iunits) {
  encoderUnits = iunits;
  return 0;
}
int32_t MockMotor::setGearing(const AbstractMotor::gearset igearset) {
  gearset = igearset;
  return 0;
}
int32_t MockMotor::setReversed(const bool) { return 0; }
int32_t MockMotor::setVoltageLimit(const std::int32_t) { return 0; }
std::shared_ptr<ContinuousRotarySensor> MockMotor::getEncoder() { return encoder; }
std::shared_ptr<MockContinuousRotarySensor> MockMotor::getMockEncoder() { return encoder; }
std::int32_t MockMotor::moveVelocity(const std::int16_t ivelocity) {
  lastVelocity = ivelocity;
  if (ivelocity > maxVelocity) { maxVelocity = ivelocity; }
  return 1;
}
std::int32_t MockMotor::moveVoltage(const std::int16_t ivoltage) {
  lastVoltage = ivoltage;
  return 1;
}
int32_t MockMotor::getCurrentDraw() { return 0; }
int32_t MockMotor::getDirection() { return 0; }
double MockMotor::getEfficiency() { return 0; }
int32_t MockMotor::isOverCurrent() { return 0; }
int32_t MockMotor::isOverTemp() { return 0; }
int32_t MockMotor::isStopped() { return 0; }
int32_t MockMotor::getZeroPositionFlag() { return 0; }
uint32_t MockMotor::getFaults() { return 0; }
uint32_t MockMotor::getFlags() { return 0; }
int32_t MockMotor::getRawPosition(std::uint32_t*) { return static_cast<int32_t>(encoder->get()); }
double MockMotor::getPower() { return 0; }
double MockMotor::getTemperature() { return 0; }
double MockMotor::getTorque() { return 0; }
int32_t MockMotor::getVoltage() { return 0; }
int32_t MockMotor::modifyProfiledVelocity(std::int32_t) { return 0; }
AbstractMotor::brakeMode MockMotor::getBrakeMode() { return brakeMode; }
int32_t MockMotor::getCurrentLimit() { return 2500; }
AbstractMotor::encoderUnits MockMotor::getEncoderUnits() { return encoderUnits; }
AbstractMotor::gearset MockMotor::getGearing() { return gearset; }
} // namespace test
