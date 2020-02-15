#include "test.hpp"

TEST_CASE("QuinticPath test") {

  SUBCASE("one point") {
    THEN("one point should return nothing") {}
    CHECK(QuinticPath({{0_in, 1_in, 90_deg}}, 1).generate(5)().size() == 0);
  }

  SUBCASE("two points") {

    GIVEN("a simple path") {
      State start {0_in, 0_in, 0_deg};
      QuinticPath path({start, {0_in, 1_in, 0_deg}}, 1);

      THEN("generating one step should return both points") {
        auto ipath = path.generate(1);
        REQUIRE(ipath().size() == 2);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(1));
      }

      THEN("generating two steps should return interpolated points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(0.5));
        CHECK(ipath()[2]->y.convert(inch) == Approx(1));
      }
    }

    GIVEN("an s curve") {
      State start {0_in, 0_in, 90_deg};
      QuinticPath path({start, {2_in, 4_in, 90_deg}}, 1);

      THEN("generating one step should return both points") {
        auto ipath = path.generate(1);
        REQUIRE(ipath().size() == 2);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->x.convert(inch) == Approx(2));
        CHECK(ipath()[1]->y.convert(inch) == Approx(4));
      }

      THEN("generating two steps should return interpolated points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->x.convert(inch) == Approx(1));
        CHECK(ipath()[1]->y.convert(inch) == Approx(2));
        CHECK(ipath()[2]->x.convert(inch) == Approx(2));
        CHECK(ipath()[2]->y.convert(inch) == Approx(4));
      }
    }
  }

  SUBCASE("three points") {

    GIVEN("a simple path") {
      State start {0_in, 0_in, 0_deg};
      QuinticPath path({start, {0_in, 1_in, 0_deg}, {0_in, 2_in, 0_deg}}, 1);

      THEN("generating one step should return 3 points") {
        auto ipath = path.generate(1);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(1));
        CHECK(ipath()[2]->y.convert(inch) == Approx(2));
      }

      THEN("generating two steps should return 5 points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 5);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(0.5));
        CHECK(ipath()[2]->y.convert(inch) == Approx(1));
        CHECK(ipath()[3]->y.convert(inch) == Approx(1.5));
        CHECK(ipath()[4]->y.convert(inch) == Approx(2));
      }
    }
  }
}