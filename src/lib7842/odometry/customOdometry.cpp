#include "customOdometry.hpp"

namespace lib7842 {

CustomOdometry::CustomOdometry(
  std::shared_ptr<ChassisModel> imodel, const ChassisScales& ichassisScales) :
  model(imodel), chassisScales(ichassisScales) {}

/**
 * Odometry algorithm provided courtecy of Michael from team 4911A, and was inspired by team 5225A
 */
void CustomOdometry::step() {
  auto newTicks = model->getSensorVals();

  double dLeftMeter = (newTicks[0] - lastTicks[0]) /
                      chassisScales.straight; // The amount the left side of the robot moved
  double dRightMeter = (newTicks[1] - lastTicks[1]) /
                       chassisScales.straight; // The amount the right side of the robot moved
  double dBackMeter = (newTicks[2] - lastTicks[2]) /
                      chassisScales.middle; // The amount the back side of the robot moved

  // Update the last values
  lastTicks = newTicks;

  double chassisWidth = chassisScales.wheelTrack.convert(meter);
  double middleDistance = chassisScales.middleWheelDistance.convert(meter);

  //change in angle orientation
  double dTheta = (dLeftMeter - dRightMeter) / chassisWidth;

  //arc radii
  double centerXArcRadius = (dBackMeter / dTheta) + middleDistance;
  double centerYArcRadius = (dRightMeter / dTheta) + chassisWidth / 2;

  //local coordinates
  double dX;
  double dY;
  if (dTheta == 0) {
    dX = dBackMeter;
    dY = dRightMeter;
  } else {
    //math
    //http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf
    dX = 2 * std::sin(dTheta / 2) * centerXArcRadius;
    dY = 2 * std::sin(dTheta / 2) * centerYArcRadius;
  }

  state.x = (dX * std::cos(state.theta.convert(radian)) +
             dY * std::sin(state.theta.convert(radian)) + state.x.convert(meter)) *
            meter;
  state.y = (dY * std::cos(state.theta.convert(radian)) -
             dX * std::sin(state.theta.convert(radian)) + state.y.convert(meter)) *
            meter;

  state.theta += dTheta * radian;
}

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

void CustomOdometry::loop() {
  while (true) {
    step();
    pros::delay(5);
  }
}

} // namespace lib7842