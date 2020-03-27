#include "../test/include/test.hpp"

TEST_CASE("PursuitLimits") {
  QTime accelTime = 1_s;
  QTime decelTime = 2_s;

  SUBCASE("Real world units") {
    QSpeed min = 0.5_mps;
    QSpeed max = 1_mps;
    QSpeed final = 0.6_mps;
    QAcceleration accel = (max - min) / accelTime;
    QAcceleration decel = (max - final) / decelTime;

    SUBCASE("Full") {
      PursuitLimits limits(min, accel, max, decel, final);
      REQUIRE(limits.minVel == min);
      REQUIRE(limits.accel == accel);
      REQUIRE(limits.maxVel == max);
      REQUIRE(limits.decel == decel);
      REQUIRE(limits.finalVel == final);
    }

    SUBCASE("Half") {
      PursuitLimits limits(min, accel, max);
      REQUIRE(limits.minVel == min);
      REQUIRE(limits.accel == accel);
      REQUIRE(limits.maxVel == max);
      REQUIRE(limits.decel == accel);
      REQUIRE(limits.finalVel == min);
    }

    SUBCASE("Full with time") {
      PursuitLimits limits(min, accelTime, max, decelTime, final);
      REQUIRE(limits.minVel == min);
      REQUIRE(limits.accel == accel);
      REQUIRE(limits.maxVel == max);
      REQUIRE(limits.decel == decel);
      REQUIRE(limits.finalVel == final);
    }

    SUBCASE("Half with time") {
      PursuitLimits limits(min, accelTime, max);
      REQUIRE(limits.minVel == min);
      REQUIRE(limits.accel == accel);
      REQUIRE(limits.maxVel == max);
      REQUIRE(limits.decel == accel);
      REQUIRE(limits.finalVel == min);
    }
  }

  SUBCASE("Motor percentages") {
    QLength diam = 4_in;
    QAngularSpeed gearset = 200_rpm;
    double min = 0.1;
    double max = 1;
    double final = 0.2;

    double accel = accelTime.convert(second);
    double decel = decelTime.convert(second);

    double maxSpeed = 2.12790542403149;

    SUBCASE("Full with wheel dimensions") {
      PursuitLimits limits(diam, gearset, min, accelTime, max, decelTime, final);
      REQUIRE(limits.minVel.convert(mps) == Approx(maxSpeed * min));
      REQUIRE(limits.accel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.maxVel.convert(mps) == Approx(maxSpeed * max));
      REQUIRE(limits.decel.convert(mps2) == Approx((max - final) * maxSpeed / decel));
      REQUIRE(limits.finalVel.convert(mps) == Approx(maxSpeed * final));
    }

    SUBCASE("Half with wheel dimensions") {
      PursuitLimits limits(diam, gearset, min, accelTime, max);
      REQUIRE(limits.minVel.convert(mps) == Approx(maxSpeed * min));
      REQUIRE(limits.accel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.maxVel.convert(mps) == Approx(maxSpeed * max));
      REQUIRE(limits.decel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.finalVel.convert(mps) == Approx(maxSpeed * min));
    }

    SUBCASE("Full with top speed") {
      PursuitLimits limits(maxSpeed * mps, min, accelTime, max, decelTime, final);
      REQUIRE(limits.minVel.convert(mps) == Approx(maxSpeed * min));
      REQUIRE(limits.accel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.maxVel.convert(mps) == Approx(maxSpeed * max));
      REQUIRE(limits.decel.convert(mps2) == Approx((max - final) * maxSpeed / decel));
      REQUIRE(limits.finalVel.convert(mps) == Approx(maxSpeed * final));
    }

    SUBCASE("Half with top speed") {
      PursuitLimits limits(maxSpeed * mps, min, accelTime, max);
      REQUIRE(limits.minVel.convert(mps) == Approx(maxSpeed * min));
      REQUIRE(limits.accel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.maxVel.convert(mps) == Approx(maxSpeed * max));
      REQUIRE(limits.decel.convert(mps2) == Approx((max - min) * maxSpeed / accel));
      REQUIRE(limits.finalVel.convert(mps) == Approx(maxSpeed * min));
    }
  }
}