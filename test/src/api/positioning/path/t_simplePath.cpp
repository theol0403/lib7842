#include "test.hpp"

SCENARIO("SimpePath test") {

  GIVEN("a point") {
    Vector point1 {5_in, 3_in};

    THEN("all the constructors should work") {
      SimplePath();
      SimplePath({point1});
      SimplePath({point1, point1});
      SimplePath(std::vector<Vector>({point1, point1}));
    }

    GIVEN("a path with two points") {
      SimplePath path({point1, point1});

      GIVEN("a path copied with the assignment operator") {
        SimplePath ipath = path;

        THEN("the size of the path should be two") {
          REQUIRE(ipath().size() == 2);
        }

        THEN("the points should be equal") {
          for (auto&& point : ipath()) {
            CHECK(*point == point1);
          }
        }

        THEN("the pointers to the points should be equal") {
          SimplePath ipath2 = path;
          for (size_t i = 0; i < ipath().size(); i++) {
            CHECK(ipath()[i] == ipath2()[i]);
            CHECK(ipath()[i] == path()[i]);
          }
        }
      }

      GIVEN("a path copied with the copy function") {
        SimplePath ipath = path.copy();

        THEN("the size of the path should be two") {
          REQUIRE(ipath().size() == 2);
        }

        THEN("the pointers should not be equal but the points should be equal") {
          for (size_t i = 0; i < path().size(); i++) {
            // pointers should not be equal
            CHECK(path()[i] != ipath()[i]);
            // values should be equal
            CHECK(*path()[i] == point1);
          }
        }
      }
    }

    GIVEN("a path with three points") {
      SimplePath path({point1, point1, point1});

      GIVEN("a path generated from the path") {
        SimplePath ipath = path.generate();

        THEN("the size of the path should be three") {
          REQUIRE(ipath().size() == 3);
        }

        THEN("the points should be equal") {
          for (auto&& point : ipath()) {
            CHECK(*point == point1);
          }
        }

        THEN("the pointers should not be equal") {
          for (size_t i = 0; i < ipath().size(); i++) {
            // pointers should not be equal
            CHECK(path()[i] != ipath()[i]);
            // values should be equal
            CHECK(*path()[i] == point1);
          }
        }
      }

      GIVEN("a path distance generated from the path") {
        SimplePath ipath = path.generate(5_in);

        THEN("the size of the path should be one") {
          REQUIRE(ipath().size() == 1);
        }

        THEN("the points should be equal") {
          for (auto&& point : ipath()) {
            CHECK(*point == point1);
          }
        }

        THEN("the pointers should not be equal") {
          for (size_t i = 0; i < ipath().size(); i++) {
            // pointers should not be equal
            CHECK(path()[i] != ipath()[i]);
            // values should be equal
            CHECK(*path()[i] == point1);
          }
        }
      }

      THEN("generating with invalid parameters should throw") {
        CHECK_THROWS_AS(path.generate(0), std::runtime_error);
        CHECK_THROWS_AS(path.generate(0.1), std::runtime_error);
      }

      GIVEN("a path generated with two interpolations") {
        SimplePath ipath2 = path.generate(2);

        THEN("the size of the path should be five") {
          REQUIRE(ipath2().size() == 5);
        }

        THEN("the points should still be equal") {
          for (auto&& point : ipath2()) {
            CHECK(*point == point1);
          }
        }
      }

      GIVEN("a path generated with five interpolations") {
        SimplePath ipath2 = path.generate(5);

        THEN("the size of the path should be eleven") {
          REQUIRE(ipath2().size() == 11);
        }

        THEN("the points should still be equal") {
          for (auto&& point : ipath2()) {
            CHECK(*point == point1);
          }
        }
      }
    }

    GIVEN("a path generated from a single point") {
      SimplePath ipath = SimplePath({point1}).generate();

      THEN("the size of the path should be one") {
        REQUIRE(ipath().size() == 1);
      }

      THEN("the point in the path should be equal to the point") {
        CHECK(*ipath()[0] == point1);
      }
    }
  }

  GIVEN("an empty path") {
    SimplePath path;

    THEN("generating should not return anything") {
      CHECK(path.generate(1)().size() == 0);
      CHECK(path.generate(5)().size() == 0);
      CHECK_THROWS_AS(path.generate(0), std::runtime_error);
    }
  }

  GIVEN("A path with 4 segments and 10 interpolations") {
    SimplePath path =
      SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}, {10_in, 3_in}}).generate(10);

    THEN("the size of the path should be 31") {
      REQUIRE(path().size() == 31);
    }
  }

  GIVEN("a generated and interpolated path") {
    SimplePath path = SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}).generate(10);

    THEN("the size of the path should be 21") {
      REQUIRE(path().size() == 21);
    }
  }

  GIVEN("a path with three segments generated by distance") {
    SimplePath path = SimplePath({{0_in, 0_in}, {6_in, 0_in}, {12_in, 0_in}}).generate(1_in);

    THEN("the path should have 13 points") {
      CHECK(path().size() == 13);
    }

    THEN("the points should increase in position") {
      for (size_t i = 0; i < path().size(); i++) {
        CHECK(path()[i]->x.convert(inch) == Approx(i));
      }
    }
  }
}
