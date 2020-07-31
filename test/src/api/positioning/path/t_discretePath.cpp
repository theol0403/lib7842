#include "../test/include/test.hpp"

TEST_CASE("SimplePath") {

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
            CHECK(ipath().at(i) == ipath2().at(i));
            CHECK(ipath().at(i) == path().at(i));
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
            CHECK(path().at(i) != ipath().at(i));
            // values should be equal
            CHECK(*path().at(i) == point1);
          }
        }
      }

      GIVEN("one angle applied") {
        StatePath statep(path, {90_deg});

        THEN("the angle should be duplicated") {
          REQUIRE(statep().at(0)->theta == 90_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
        }
      }

      GIVEN("two angles applied") {
        StatePath statep(path, {0_deg, 90_deg});

        THEN("the angles should be applied directly") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
        }
      }

      GIVEN("three angles applied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg});

        THEN("the middle angle should be dropped") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
        }
      }

      GIVEN("four angles applied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg, 180_deg});

        THEN("the middle angles should be dropped") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 180_deg);
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
            CHECK(path().at(i) != ipath().at(i));
            // values should be equal
            CHECK(*path().at(i) == point1);
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
            CHECK(path().at(i) != ipath().at(i));
            // values should be equal
            CHECK(*path().at(i) == point1);
          }
        }
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

      GIVEN("one angle applied") {
        StatePath statep(path, {90_deg});

        THEN("the angle should be duplicated") {
          REQUIRE(statep().at(0)->theta == 90_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
          REQUIRE(statep().at(2)->theta == 90_deg);
        }
      }

      GIVEN("two angles applied") {
        StatePath statep(path, {0_deg, 90_deg});

        THEN("the angles should be interpolated") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 45_deg);
          REQUIRE(statep().at(2)->theta == 90_deg);
        }
      }

      GIVEN("three angles applied") {
        StatePath statep(path, {0_deg, 50_deg, 90_deg});

        THEN("the angles should be copied") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 50_deg);
          REQUIRE(statep().at(2)->theta == 90_deg);
        }
      }

      GIVEN("four angles applied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg, 180_deg});

        THEN("positional interpolation") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 67.5_deg);
          REQUIRE(statep().at(2)->theta == 180_deg);
        }
      }

      GIVEN("five angles applied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg, 180_deg, 360_deg});

        THEN("positional interpolation") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
          REQUIRE(statep().at(2)->theta == 360_deg);
        }
      }
    }

    GIVEN("a path with four points") {
      SimplePath path({point1, point1, point1, point1});

      GIVEN("one angle applied") {
        StatePath statep(path, {90_deg});

        THEN("the angle should be duplicated") {
          REQUIRE(statep().at(0)->theta == 90_deg);
          REQUIRE(statep().at(1)->theta == 90_deg);
          REQUIRE(statep().at(2)->theta == 90_deg);
          REQUIRE(statep().at(3)->theta == 90_deg);
        }
      }

      GIVEN("two angles applied") {
        StatePath statep(path, {0_deg, 90_deg});

        THEN("the angles should be interpolated") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 30_deg);
          REQUIRE(statep().at(2)->theta == 60_deg);
          REQUIRE(statep().at(3)->theta == 90_deg);
        }
      }

      GIVEN("three angles applied") {
        StatePath statep(path, {0_deg, 50_deg, 90_deg});

        THEN("the angles should be interpolated") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 2.0 / 3.0 * 50_deg);
          REQUIRE(statep().at(2)->theta == 50_deg + (1.0 / 3.0 * 40_deg));
          REQUIRE(statep().at(3)->theta == 90_deg);
        }
      }

      GIVEN("the angles should be copied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg, 180_deg});

        THEN("positional interpolation") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 45_deg);
          REQUIRE(statep().at(2)->theta == 90_deg);
          REQUIRE(statep().at(3)->theta == 180_deg);
        }
      }

      GIVEN("five angles applied") {
        StatePath statep(path, {0_deg, 45_deg, 90_deg, 180_deg, 360_deg});

        THEN("positional interpolation") {
          REQUIRE(statep().at(0)->theta == 0_deg);
          REQUIRE(statep().at(1)->theta == 45_deg + (1.0 / 3.0 * 45_deg));
          REQUIRE(statep().at(2)->theta == 90_deg + (2.0 / 3.0 * 90_deg));
          REQUIRE(statep().at(3)->theta == 360_deg);
        }
      }
    }

    GIVEN("a path generated from a single point") {
      SimplePath ipath = SimplePath({point1}).generate();

      THEN("the size of the path should be one") {
        REQUIRE(ipath().size() == 1);
      }

      THEN("the point in the path should be equal to the point") {
        CHECK(*ipath().at(0) == point1);
      }
    }
  }

  GIVEN("an empty path") {
    SimplePath path;

    THEN("generating should not return anything") {
      CHECK(path.generate(1)().size() == 0);
      CHECK(path.generate(5)().size() == 0);
      CHECK(path.generate(0)().size() == 0);
    }
  }

  GIVEN("A path with 4 segments and 10 interpolations") {
    SimplePath path =
      SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}, {10_in, 3_in}}).generate(10);

    THEN("the size of the path should be 31") {
      REQUIRE(path().size() == 31);
    }
  }

  GIVEN("a path with three segments") {
    SimplePath path({{0_in, 0_in}, {2.5_in, 4_in}, {5_in, 0_in}});

    WHEN("the path is smoothed") {
      path.smoothen(0.25, 0.0001_in);

      THEN("the starting point should not move") {
        CHECK(*path().at(0) == (Vector {0_in, 0_in}));
      }

      THEN("the ending point should not move") {
        CHECK(*path().at(2) == (Vector {5_in, 0_in}));
      }

      THEN("the middle point x should not move") {
        CHECK(path().at(1)->x == 2.5_in);
      }

      THEN("the middle point y should move") {
        CHECK(path().at(1)->y != 4_in);
      }
    }
  }

  GIVEN("a path with many segments") {
    SimplePath ipath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}});

    WHEN("the path is smoothed") {
      SimplePath path = ipath.copy().smoothen(0.25, 0.0001_in);

      THEN("none of the middle points should be the same") {
        for (size_t i = 1; i < path().size() - 1; i++) {
          CHECK(path().at(i)->x != ipath().at(i)->x);
        }
      }
    }
  }

  GIVEN("a generated and interpolated path") {
    SimplePath path = SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}})
                        .generate(10)
                        .smoothen(0.25, 1e-10 * inch);

    THEN("the size of the path should be 21") {
      REQUIRE(path().size() == 21);
    }

    THEN("the 10th point should be in a certain position") {
      CHECK(path().at(10)->x.convert(inch) == Approx(5));
      CHECK(path().at(10)->y.convert(inch) == Approx(3.5).epsilon(0.001));
    }
  }

  GIVEN("a path with three segments generated by distance") {
    SimplePath path = SimplePath({{0_in, 0_in}, {6_in, 0_in}, {12_in, 0_in}}).generate(1_in);

    THEN("the path should have 13 points") {
      CHECK(path().size() == 13);
    }

    THEN("the points should increase in position") {
      for (size_t i = 0; i < path().size(); i++) {
        CHECK(path().at(i)->x.convert(inch) == Approx(i));
      }
    }
  }

  GIVEN("a path with angles") {
    StatePath path =
      StatePath({{0_in, 0_in, 0_deg}, {0_in, 1_ft, 90_deg}, {0_in, 2_ft, 180_deg}}).generateT(10);

    THEN("the path should have 21 points") {
      CHECK(path().size() == 21);
    }

    THEN("the angles should be interpolated") {
      for (size_t i = 0; i < path().size(); i++) {
        CHECK(path().at(i)->y.convert(foot) == Approx(i / 10.0));
        CHECK(path().at(i)->x.convert(foot) == 0);
        CHECK(path().at(i)->theta.convert(degree) == Approx(i * (90.0 / 10.0)));
      }
    }
  }
}
