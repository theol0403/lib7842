#include "test.hpp"

static QLength calculateDistanceTraveled(int ticks) {
  return (ticks / 360.0) * 1_pi * 4_in;
}

static void assertOdomState(const State& istate, std::shared_ptr<CustomOdometry> iodom) {
  CHECK(iodom->getState().x.convert(meter) == Approx(istate.x.convert(meter)));
  CHECK(iodom->getState().y.convert(meter) == Approx(istate.y.convert(meter)));
  CHECK(iodom->getState().theta.convert(degree) == Approx(istate.theta.convert(degree)));
}

TEST_CASE("CustomOdometry test") {
  auto model = std::make_shared<MockThreeEncoderXDriveModel>();

  SUBCASE("Common Odometry Tests") {

    auto odom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}), createTimeUtil());

    auto assertState = [&](const State& istate) {
      assertOdomState(istate, odom);
    };

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
      model, ChassisScales({{2.75_in, 12.9_in, 0.5_in, 2.75_in}, quadEncoderTPR}),
      createTimeUtil());

    model->setSensorVals(0, 0, 0);
    smallOdom->step();
    assertOdomState({0_in, 0_in, 0_deg}, smallOdom);

    model->setSensorVals(0, -2, 0);
    smallOdom->step();
    assertOdomState({0.00181102_in, -0.024_in, 0.21317829_deg}, smallOdom);
  }

  SUBCASE("NinetyDegreePivotTurn") {
    auto straightOdom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(450, 0, 0);
    straightOdom->step();
    assertOdomState({5_in, 5_in, 90_deg}, straightOdom);
  }

  SUBCASE("FullPivotRotation") {
    auto straightOdom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(1800, 0, 0);
    straightOdom->step();
    assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
  }

  SUBCASE("FullPointRotation") {
    auto straightOdom = std::make_shared<CustomOdometry>(
      model, ChassisScales({{4_in, 10_in, 0_in, 4_in}, 360}), createTimeUtil());

    assertOdomState({0_m, 0_m, 0_deg}, straightOdom);

    model->setSensorVals(900, -900, 0);
    straightOdom->step();
    assertOdomState({0_in, 0_in, 360_deg}, straightOdom);
  }
}
