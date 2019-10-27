#include "customOdometry.hpp"

namespace lib7842 {

CustomOdometry::CustomOdometry(
  std::shared_ptr<ChassisModel> imodel, const ChassisScales& ichassisScales) :
  model(imodel), chassisScales(ichassisScales) {}

void CustomOdometry::step() {
  auto newTicks = model->getSensorVals();

  double L = (newTicks[0] - lastTicks[0]) /
             chassisScales.straight; // The amount the left side of the robot moved
  double R = (newTicks[1] - lastTicks[1]) /
             chassisScales.straight; // The amount the right side of the robot moved
  double S = (newTicks[2] - lastTicks[2]) /
             chassisScales.middle; // The amount the back side of the robot moved

  std::cout << "L: " << L << ", R: " << R << ", S: " << S << std::endl;

  // Update the last values
  lastTicks = newTicks;

  double chassisWidth = chassisScales.wheelTrack.convert(meter);

  // The hypotenuse of the triangle formed by the middle of the robot on the
  // starting position and ending position and the middle of the circle it travels around
  double h;
  double i; // Half on the angle that I've traveled
  double h2; // The same as h but using the back instead of the side wheels
  double a = (L - R) / chassisWidth; // The angle that I've traveled

  std::cout << "A: " << (a * radian).convert(degree) << std::endl;

  if (a) {
    // The radius of the circle the robot travel's around with the right side of the robot
    double r = R / a;
    i = a / 2.0;
    double sinI = std::sin(i);
    h = ((r + (chassisWidth / 2.0)) * sinI) * 2.0;

    // The radius of the circle the robot travel's around with the back of the robot
    double r2 = S / a;
    h2 = ((r2 + chassisScales.middleWheelDistance.convert(meter)) * sinI) * 2.0;

  } else {
    h = R;
    i = 0;
    h2 = S;
  }

  double p = i + state.theta.convert(radian); // The global ending angle of the robot
  double cosP = std::cos(p);
  double sinP = std::sin(p);

  // Update the global position
  state.y += (h * cosP) * meter;
  state.x += (h * sinP) * meter;

  state.y += (h2 * -sinP) * meter; // -sin(x) = sin(-x)
  state.x += (h2 * cosP) * meter; // cos(x) = cos(-x)

  state.theta += a * radian;
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