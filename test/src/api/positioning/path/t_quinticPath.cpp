#include "test.hpp"

TEST_CASE("QuinticPath test") {

  GIVEN("a simple path") {
    State start {0_in, 0_in, 0_deg};
    QuinticPath path({start, {0_in, 1_in, 0_deg}}, 1);

    THEN("generating one step should return both points") {
      auto ipath = path.generate(1);
      CHECK(ipath().size() == 2);
      CHECK(*ipath()[0] == start);
      CHECK(ipath()[1]->y.convert(inch) == Approx(1));
    }

    THEN("generating two steps should return interpolated points") {
      auto ipath = path.generate(2);
      CHECK(ipath().size() == 3);
      CHECK(*ipath()[0] == start);
      CHECK(ipath()[1]->y.convert(inch) == Approx(0.5));
      CHECK(ipath()[2]->y.convert(inch) == Approx(1));
    }
  }
}