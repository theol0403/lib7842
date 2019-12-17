#include "customOdometry.hpp"

namespace lib7842 {

CustomOdometry::CustomOdometry(const std::shared_ptr<ChassisModel>& imodel,
                               const ChassisScales& ichassisScales, const TimeUtil& itimeUtil,
                               const std::shared_ptr<Logger>& ilogger) :
  TaskWrapper(ilogger),
  model(imodel),
  chassisScales(ichassisScales),
  chassisWidth(chassisScales.wheelTrack.convert(meter)),
  middleDistance(chassisScales.middleWheelDistance.convert(meter)),
  timeUtil(itimeUtil),
  logger(ilogger) {}

const State& CustomOdometry::getState() const {
  return state;
}

void CustomOdometry::setState(const State& istate) {
  state = istate;
}

void CustomOdometry::resetState() {
  state = {0_in, 0_in, 0_deg};
}

void CustomOdometry::reset() {
  model->resetSensors();
  lastTicks = {0, 0, 0};
  resetState();
}

void CustomOdometry::setScales(const ChassisScales& ichassisScales) {
  chassisScales = ichassisScales;
  chassisWidth = chassisScales.wheelTrack.convert(meter);
  middleDistance = chassisScales.middleWheelDistance.convert(meter);
}

/**
 * Odometry algorithm provided courtesy of the pilons from team 5225A
 */
void CustomOdometry::step() {
  auto newTicks = model->getSensorVals();

  if (newTicks.size() < 3) {
    std::string msg("CustomOdometry::step: The model does not contain three encoders");
    LOG_ERROR(msg);
    throw std::runtime_error(msg);
  }

  // The amount the left side of the robot moved
  double L = (newTicks[0] - lastTicks[0]) / chassisScales.straight;
  // The amount the right side of the robot moved
  double R = (newTicks[1] - lastTicks[1]) / chassisScales.straight;
  // The amount the back side of the robot moved
  double S = (newTicks[2] - lastTicks[2]) / chassisScales.middle;

  // Update the last values
  lastTicks = newTicks;

  // The hypotenuse of the triangle formed by the middle of the robot on the starting position and ending position and the middle of the circle it travels around
  double h;
  double i; // Half on the angle that I've traveled
  double h2; // The same as h but using the back instead of the side wheels
  double a = (L - R) / chassisWidth; // The angle that I've traveled
  if (a) {
    // The radius of the circle the robot travel's around with the right side of the robot
    double r = R / a;
    i = a / 2.0;
    double sinI = std::sin(i);
    h = ((r + (chassisWidth / 2)) * sinI) * 2.0;

    // The radius of the circle the robot travel's around with the back of the robot
    double r2 = S / a;
    h2 = ((r2 + middleDistance) * sinI) * 2.0;
  } else {
    h = R;
    i = 0;

    h2 = S;
  }
  double p = i + state.theta.convert(radian); // The global ending angle of the robot
  double cosP = std::cos(p);
  double sinP = std::sin(p);

  // Update the global position
  state.y += ((h * cosP) + (h2 * -sinP)) * meter;
  state.x += ((h * sinP) + (h2 * cosP)) * meter;

  state.theta += a * radian;
}

std::shared_ptr<ReadOnlyChassisModel> CustomOdometry::getModel() {
  return model;
}

/**
 * @return The internal ChassisScales.
 */
ChassisScales CustomOdometry::getScales() {
  return chassisScales;
}

void CustomOdometry::loop() {
  auto rate = timeUtil.getRate();
  while (true) {
    step();
    rate->delayUntil(5_ms);
  }
}

OdomState CustomOdometry::getState(const StateMode& imode) const {
  const State& istate = getState();
  if (imode == StateMode::CARTESIAN) {
    return {istate.x, istate.y, istate.theta};
  } else {
    return {istate.y, istate.x, istate.theta};
  }
}

void CustomOdometry::setState(const OdomState& istate, const StateMode& imode) {
  if (imode == StateMode::CARTESIAN) {
    state = {istate.x, istate.y, istate.theta};
  } else {
    state = {istate.y, istate.x, istate.theta};
  }
}

} // namespace lib7842