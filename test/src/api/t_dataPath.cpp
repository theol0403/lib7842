#include "test.hpp"

SCENARIO("DataPath test") {

  GIVEN("a data point with some data") {
    DataPoint point1 {5_in, 3_in};

    point1.setData("curvature", 5.0);
    point1.setData("distance", 5_m);
    point1.setData("velocity", 5_mps);
    point1.setData("segmentIndex", 5);

    THEN("constructors should work") {
      DataPath();
      DataPath({point1});
      DataPath({point1, point1});
      DataPath(std::vector<DataPoint>({point1, point1}));
    }

    GIVEN("a path containing three points") {
      DataPath path({point1, point1, point1});

      THEN("the size of the path should be three") {
        REQUIRE(path().size() == 3);
      }

      THEN("each point should contain data") {
        for (auto&& point : path()) {
          CHECK(point->getData<double>("curvature") == 5.0);
          CHECK(point->getData<QLength>("distance") == 5_m);
          CHECK(point->getData<QSpeed>("velocity") == 5_mps);
          CHECK(point->getData<int>("segmentIndex") == 5);
        }
      }

      GIVEN("a simple path generated from the data path") {
        SimplePath ipath = path.generate();

        THEN("the size of the path should be three") {
          REQUIRE(ipath().size() == 3);
        }

        THEN("the positions should be the same") {
          for (auto&& point : ipath()) {
            CHECK(*point == point1);
          }
        }
      }
    }
  }
}
