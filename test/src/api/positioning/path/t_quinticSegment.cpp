#include "test.hpp"

SCENARIO("QuinticSegment test") {

  GIVEN("a simple segment") {
    DataState start {0_in, 0_in, 0_deg};
    start.setData("slope", 1.0);
    DataState end {0_in, 1_in, 0_deg};
    end.setData("slope", 1.0);
    QuinticSegment segment(start, end);

    THEN("generating one step should return both points") {
      auto path = segment.generate(1);
      CHECK(path().size() == 2);
      CHECK(*path()[0] == start);
      CHECK(path()[1]->y.convert(inch) == Approx(1));
    }

    THEN("generating two steps should return interpolated points") {
      auto path = segment.generate(2);
      CHECK(path().size() == 3);
      CHECK(*path()[0] == start);
      CHECK(path()[1]->y.convert(inch) == Approx(0.5));
      CHECK(path()[2]->y.convert(inch) == Approx(1));
    }
  }

  GIVEN("an s curve") {
    DataState start {0_in, 0_in, 90_deg};
    start.setData("slope", 1.0);
    DataState end {2_in, 4_in, 90_deg};
    end.setData("slope", 1.0);
    QuinticSegment segment(start, end);

    THEN("generating one step should return both points") {
      auto path = segment.generate(1);
      CHECK(path().size() == 2);
      CHECK(*path()[0] == start);
      CHECK(path()[1]->x.convert(inch) == Approx(2));
      CHECK(path()[1]->y.convert(inch) == Approx(4));
    }

    THEN("generating two steps should return interpolated points") {
      auto path = segment.generate(2);
      CHECK(path().size() == 3);
      CHECK(*path()[0] == start);
      CHECK(path()[1]->x.convert(inch) == Approx(1));
      CHECK(path()[1]->y.convert(inch) == Approx(2));
      CHECK(path()[2]->x.convert(inch) == Approx(2));
      CHECK(path()[2]->y.convert(inch) == Approx(4));
    }
  }
}