#include "test.hpp"

SCENARIO("Vector test") {

  GIVEN("a default-constructed point") {
    Vector point;
    THEN("the members should be equal to 0") {
      CHECK(point == (Vector {0_in, 0_in}));
    }
  }

  GIVEN("a point") {
    Vector point {5_in, 2_in};

    THEN("the point should contain the proper info") {
      CHECK(point.x == 5_in);
      CHECK(point.y == 2_in);
    }

    WHEN("the copy constructor is called") {
      Vector point2(point);

      THEN("the new point should be equal to the old point") {
        CHECK(point2.x == point.x);
        CHECK(point2.y == point.y);
      }
    }

    GIVEN("a point added to itself") {
      Vector point2 = point + point;

      THEN("the new point members should be twice the old point members") {
        CHECK(point2.x == point.x * 2);
        CHECK(point2.y == point.y * 2);
      }

      THEN("the new point should be the old point times two") {
        CHECK(point2 == point * 2);
      }

      THEN("the new point should be the old point divided by .5") {
        CHECK(point2 == point / 0.5);
      }
    }

    THEN("the point should be equal to itself") {
      CHECK(point == point);
    }

    THEN("the point plus another point should be equal to itself") {
      CHECK((point + Vector {1_in, 2_in}) == (point + Vector {1_in, 2_in}));
      CHECK((point + point) == (point + Vector {5_in, 2_in}));
    }

    THEN("the point should not be equal to some different points") {
      CHECK(point != point + point);
      CHECK(point != point + (Vector {0_in, 4_in}));
    }

    THEN("the accessor operators should work") {
      CHECK(point.at(0) == point.x);
      CHECK(point.at(1) == point.y);
      CHECK_THROWS_AS(point.at(2), std::runtime_error);
      CHECK_THROWS_AS(point.at(5), std::runtime_error);
      CHECK_THROWS_AS(point.at(-1), std::runtime_error);
    }
  }
}
