#include "test.hpp"

SCENARIO("DataPoint test") {

  GIVEN("a data point") {
    DataPoint point {5_in, 2_in};

    THEN("constructors should work") {
      DataPoint();
      DataPoint(5_in, 2_in);
      DataPoint pointCopy(point);
    }

    GIVEN("some data") {
      point.setData("curvature", 5.0);
      point.setData("distance", 5_m);
      point.setData("velocity", 5_mps);
      point.setData("segmentIndex", 5);

      THEN("the data should be accessible") {
        CHECK(point.getData<double>("curvature") == 5.0);
        CHECK(point.getData<QLength>("distance") == 5_m);
        CHECK(point.getData<QSpeed>("velocity") == 5_mps);
        CHECK(point.getData<int>("segmentIndex") == 5);
      }

      THEN("the point should be equal to itself") {
        CHECK(point == point);
      }
    }

    GIVEN("some bad data") {
      point.setData("curvature", 5_m);
      point.setData("distance", 5);
      point.setData("velocity", 5);
      point.setData("segmentIndex", 5_mps);

      THEN("accessing the data should throw") {
        CHECK_THROWS_AS(point.getData<double>("curvature"), std::runtime_error);
        CHECK_THROWS_AS(point.getData<QLength>("distance"), std::runtime_error);
        CHECK_THROWS_AS(point.getData<QSpeed>("velocity"), std::runtime_error);
        CHECK_THROWS_AS(point.getData<int>("segmentIndex"), std::runtime_error);
      }
    }

    THEN("accessing nonexistent data should throw") {
      CHECK_THROWS_AS(point.getData<double>("curvature"), std::runtime_error);
      CHECK_THROWS_AS(point.getData<QLength>("distance"), std::runtime_error);
      CHECK_THROWS_AS(point.getData<QSpeed>("velocity"), std::runtime_error);
      CHECK_THROWS_AS(point.getData<int>("segmentIndex"), std::runtime_error);
    }
  }
}
