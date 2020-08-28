#include "lib7842/api/odometry/customOdometry.hpp"
#include <cmath>

namespace lib7842 {

CustomOdometry::CustomOdometry(std::shared_ptr<ChassisModel> imodel,
                               const ChassisScales& ichassisScales) :
  model(std::move(imodel)), chassisScales(ichassisScales) {}

void CustomOdometry::setScales(const ChassisScales& ichassisScales) {
  chassisScales = ichassisScales;
}

/**
 * Odometry algorithm provided courtesy of the pilons from team 5225A
 */
void CustomOdometry::step() {
  auto newTicks = model->getSensorVals();

  if (newTicks.size() < 3) {
    GLOBAL_ERROR_THROW("CustomOdometry::step: The model does not contain three encoders");
  }

  // The amount the left side of the robot moved
  double L = (newTicks[0] - lastTicks[0]) / chassisScales.straight;
  // The amount the right side of the robot moved
  double R = (newTicks[1] - lastTicks[1]) / chassisScales.straight;
  // The amount the back side of the robot moved
  double S = (newTicks[2] - lastTicks[2]) / chassisScales.middle;

  // Update the last values
  lastTicks = newTicks;

  // The hypotenuse of the triangle formed by the middle of the robot on the starting position and
  // ending position and the middle of the circle it travels around
  double h = 0;
  double i = 0; // Half on the angle that I've traveled
  double h2 = 0; // The same as h but using the back instead of the side wheels
  double a = (L - R) / chassisScales.wheelTrack.convert(meter); // The angle that I've traveled
  if (a != 0.0) {
    // The radius of the circle the robot travels around with the right side of the robot
    double r = R / a;
    i = a / 2.0;
    double sinI = std::sin(i);
    h = ((r + (chassisScales.wheelTrack.convert(meter) / 2)) * sinI) * 2.0;

    // The radius of the circle the robot travels around with the back of the robot
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
  state.y += ((h * cosP) + (h2 * -sinP)) * meter;
  state.x += ((h * sinP) + (h2 * cosP)) * meter;

  state.theta += a * radian;
}

const State& CustomOdometry::getState() const { return state; }

OdomState CustomOdometry::getState(const StateMode& imode) const {
  const State& istate = getState();
  if (imode == StateMode::CARTESIAN) { return {istate.x, istate.y, istate.theta}; }
  return {istate.y, istate.x, istate.theta};
}

void CustomOdometry::setState(const State& istate) { state = istate; }

void CustomOdometry::resetState() { state = {0_in, 0_in, 0_deg}; }

void CustomOdometry::reset() {
  model->resetSensors();
  lastTicks = {0, 0, 0};
  resetState();
}

void CustomOdometry::setState(const OdomState& istate, const StateMode& imode) {
  if (imode == StateMode::CARTESIAN) {
    state = {istate.x, istate.y, istate.theta};
  } else {
    state = {istate.y, istate.x, istate.theta};
  }
}

std::shared_ptr<ReadOnlyChassisModel> CustomOdometry::getModel() { return model; }

ChassisScales CustomOdometry::getScales() { return chassisScales; }

void CustomOdometry::loop() {
  auto rate = global::getTimeUtil()->getRate();
  rate->delayUntil(20_ms);
  while (true) {
    step();
    rate->delayUntil(5_ms);
  }
}

} // namespace lib7842

#include "lib7842/test/mocks.hpp"
namespace test {
static QLength calculateDistanceTraveled(double ticks) {
  return (double(ticks) / 360.0) * 1_pi * 4_in;
}

static void assertOdomState(const State& istate, const std::shared_ptr<CustomOdometry>& iodom) {
  CHECK(iodom->getState().x.convert(meter) == Approx(istate.x.convert(meter)));
  CHECK(iodom->getState().y.convert(meter) == Approx(istate.y.convert(meter)));
  CHECK(iodom->getState().theta.convert(degree) == Approx(istate.theta.convert(degree)));
}

TEST_CASE("CustomOdometry") {
  auto model = std::make_shared<MockThreeEncoderXDriveModel>();

  SUBCASE("Common Odometry Tests") {

    auto odom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));

    auto assertState = [&](const State& istate) { assertOdomState(istate, odom); };

    SUBCASE("MockSensorsFunction") {
      model->setSensorVals(10, 20, 30);
      auto sensors = model->getSensorVals();
      CHECK(sensors[0] == 10);
      CHECK(sensors[1] == 20);
      CHECK(sensors[2] == 30);
    }

    SUBCASE("NoSensorMovementDoesNotAffectState") {
      assertState({0_m, 0_m, 0_deg});
      odom->step();
      assertState({0_m, 0_m, 0_deg});
    }

    SUBCASE("RepeatedlyCallingStep") {
      for (size_t i = 0; i < 50; i++) {
        assertState({0_m, 0_m, 0_deg});
        odom->step();
        assertState({0_m, 0_m, 0_deg});
      }
    }

    SUBCASE("MoveForwardTest") {
      model->setSensorVals(10, 10, 0);
      odom->step();
      assertState({0_m, calculateDistanceTraveled(10), 0_deg});

      model->setSensorVals(20, 20, 0);
      odom->step();
      assertState({0_m, calculateDistanceTraveled(20), 0_deg});

      model->setSensorVals(10, 10, 0);
      odom->step();
      assertState({0_m, calculateDistanceTraveled(10), 0_deg});
    }

    SUBCASE("TurnInPlaceTest") {
      model->setSensorVals(10, -10, -10);
      odom->step();
      assertState({0_m, 0_m, 4_deg});

      model->setSensorVals(0, 0, 0);
      odom->step();
      assertState({0_m, 0_m, 0_deg});

      model->setSensorVals(-10, 10, 10);
      odom->step();
      assertState({0_m, 0_m, -4_deg});
    }

    SUBCASE("TurnAndDriveTest") {
      model->setSensorVals(90, -90, -90);
      odom->step();
      assertState({0_m, 0_m, 36_deg});

      model->setSensorVals(180, 0, -90);
      odom->step();
      assertState({calculateDistanceTraveled(90) * std::sin((36_deg).convert(radian)),
                   calculateDistanceTraveled(90) * std::cos((36_deg).convert(radian)), 36_deg});
    }

    SUBCASE("StrafeTest") {
      model->setSensorVals(0, 0, 10);
      odom->step();
      assertState({calculateDistanceTraveled(10), 0_in, 0_deg});
    }

    SUBCASE("DriveForwardWhileStrafingTest") {
      model->setSensorVals(10, 10, 10);
      odom->step();
      assertState({calculateDistanceTraveled(10), calculateDistanceTraveled(10), 0_deg});
    }

    SUBCASE("FullPointRotationWithMiddle") {
      assertState({0_m, 0_m, 0_deg});

      model->setSensorVals(900, -900, -900);
      odom->step();
      assertState({0_in, 0_in, 360_deg});
    }
  }

  SUBCASE("SmallSwingTurnOnRightWheels") {
    auto smallOdom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{2.75_in, 12.9_in, 0.5_in, 2.75_in}, quadEncoderTPR}));

    model->setSensorVals(0, 0, 0);
    smallOdom->step();
    assertOdomState({0_in, 0_in, 0_deg}, smallOdom);

    model->setSensorVals(0, -2, 0);
    smallOdom->step();
    assertOdomState({0.00181102_in, -0.024_in, 0.21317829_deg}, smallOdom);
  }

  SUBCASE("NinetyDegreePivotTurn") {
    auto straightOdom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}));

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(450, 0, 0);
    straightOdom->step();
    assertOdomState({5_in, 5_in, 90_deg}, straightOdom);
  }

  SUBCASE("FullPivotRotation") {
    auto straightOdom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}));

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(1800, 0, 0);
    straightOdom->step();
    assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
  }

  SUBCASE("FullPointRotation") {
    auto straightOdom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}));

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(900, -900, 0);
    straightOdom->step();
    assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
  }
}
} // namespace test
