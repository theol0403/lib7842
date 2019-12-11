#include "test.hpp"

SCENARIO("CompoundPath test") {

  GIVEN("an empty path") {
    CompoundPath path;

    GIVEN("a point") {
      Vector point1 {5_in, 3_in};

      THEN("adding the point to the path in various ways should work") {
        path.add(std::make_shared<SimplePath>());
        path.add(CompoundPath());
        path.add((SimplePath({point1})));
        path.add(std::make_shared<SimplePath>(SimplePath({point1, point1})));
      }

      GIVEN("a path containing the point added in various ways") {
        path.add(std::make_shared<SimplePath>(SimplePath({point1})));
        path.add(SimplePath({point1, point1}));
        path.add(SimplePath({point1}));

        GIVEN("a simple path generated from the compound path") {
          SimplePath ipath = path.generate();

          THEN("the size of the path should be four") {
            REQUIRE(ipath().size() == 4);
          }

          THEN("each generated point should be equal to the original point") {
            for (auto&& point : ipath()) {
              CHECK(*point == point1);
            }
          }
        }
      }
    }

    GIVEN("a whole bunch of points and segments added in increasing order") {
      path.add(std::make_shared<SimplePath>(SimplePath({{1_in, 2_in}})));
      path.add(SimplePath({{2_in, 3_in}, {3_in, 4_in}}));

      CompoundPath segment1 =
        CompoundPath() +
        CompoundPath().add(CompoundPath().add(SimplePath({{4_in, 5_in}, {5_in, 6_in}})));

      CompoundPath segment2 = CompoundPath().add(
        CompoundPath() + std::make_shared<SimplePath>(SimplePath({{6_in, 7_in}})));

      CompoundPath segment3 =
        CompoundPath().add(CompoundPath().add(CompoundPath() + SimplePath({{7_in, 8_in}})));

      CompoundPath segment3b =
        CompoundPath() + std::make_shared<CompoundPath>() +
        std::make_shared<CompoundPath>(CompoundPath() + CompoundPath().add(segment3));

      CompoundPath segment4 =
        CompoundPath() +
        std::make_shared<CompoundPath>(
          CompoundPath().add(std::make_shared<CompoundPath>(CompoundPath().add(segment2))) +
          std::make_shared<CompoundPath>(segment3b));

      CompoundPath segment5 = CompoundPath().add(CompoundPath() + SimplePath({{8_in, 9_in}}));

      path += std::shared_ptr<CompoundPath>(&segment1, [](AbstractPath*) {}); // empty deleter
      path += CompoundPath() + std::make_shared<CompoundPath>(std::move(segment4)) +
              CompoundPath() // move the local into shared
              + segment5; // make copy

      GIVEN("a simple path generated from the compound path") {
        SimplePath ipath = path.generate();

        THEN("the size of the path should be eight") {
          REQUIRE(ipath().size() == 8);
        }

        THEN("the points should be preserved") {
          for (size_t i = 0; i < ipath().size(); i++) {
            CHECK(*ipath()[i] == (Vector {(i + 1) * inch, (i + 2) * inch}));
          }
        }
      }
    }

    GIVEN("a whole bunch of points and segments added concisely in increasing order") {
      path.add(SimplePath({{1_in, 2_in}}));
      path.add(SimplePath({{2_in, 3_in}, {3_in, 4_in}}));

      CompoundPath segment1 = CompoundPath() + SimplePath({{4_in, 5_in}, {5_in, 6_in}});
      CompoundPath segment2 = CompoundPath() + StatePath({{6_in, 7_in}});
      CompoundPath segment3 = CompoundPath() + DataPath({{7_in, 8_in}});
      CompoundPath segment4 = segment2 + segment3;
      CompoundPath segment5 = CompoundPath() + SimplePath({{8_in, 9_in}});

      path += (std::move(segment1) + std::move(segment4)) + segment5;

      GIVEN("a simple path generated from the compound path") {
        SimplePath ipath = path.generate();

        THEN("the size of the path should be eight") {
          REQUIRE(ipath().size() == 8);
        }

        THEN("the points should be preserved") {
          for (size_t i = 0; i < ipath().size(); i++) {
            CHECK(*ipath()[i] == (Vector {(i + 1) * inch, (i + 2) * inch}));
          }
        }
      }
    }
  }
}
