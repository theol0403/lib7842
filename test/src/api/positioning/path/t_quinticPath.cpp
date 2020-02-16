#include "test.hpp"

class MockQuinticPath : public QuinticPath {
public:
  using QuinticPath::QuinticPath;
  using QuinticPath::path;
};

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

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating two steps should return interpolated points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(0.5));
        CHECK(ipath()[2]->y.convert(inch) == Approx(1));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating 10 steps should return interpolated points") {
        auto ipath = path.generate(10);
        REQUIRE(ipath().size() == 11);

        for (size_t i = 0; i < ipath().size(); i++) {
          CHECK(ipath()[i]->y.convert(inch) == Approx(i * 0.1));
          CHECK(ipath()[i]->x.convert(inch) == Approx(0));
        }
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

      THEN("generating 4 steps should return interpolated points") {
        auto ipath = path.generate(4);
        REQUIRE(ipath().size() == 5);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[2]->x.convert(inch) == Approx(1));
        CHECK(ipath()[2]->y.convert(inch) == Approx(2));
        CHECK(ipath()[4]->x.convert(inch) == Approx(2));
        CHECK(ipath()[4]->y.convert(inch) == Approx(4));
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

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating two steps should return 5 points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 5);
        CHECK(*ipath()[0] == start);
        CHECK(ipath()[1]->y.convert(inch) == Approx(0.5));
        CHECK(ipath()[2]->y.convert(inch) == Approx(1));
        CHECK(ipath()[3]->y.convert(inch) == Approx(1.5));
        CHECK(ipath()[4]->y.convert(inch) == Approx(2));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating 10 steps should return interpolated points") {
        auto ipath = path.generate(10);
        REQUIRE(ipath().size() == 21);

        for (size_t i = 0; i < ipath().size(); i++) {
          CHECK(ipath()[i]->y.convert(inch) == Approx(i * 0.1));
          CHECK(ipath()[i]->x.convert(inch) == Approx(0));
        }
      }
    }
  }

  SUBCASE("many points") {
    QuinticPath path(
      {{0_in, 0_in, 0_deg}, {0_in, 1_in, 0_deg}, {0_in, 2_in, 90_deg}, {2_in, 2_in, 90_deg}}, 1);

    THEN("generating one step should return 4 points") {
      auto ipath = path.generate(1);
      REQUIRE(ipath().size() == 4);
    }

    THEN("generating two steps should return 7 points") {
      auto ipath = path.generate(2);
      REQUIRE(ipath().size() == 7);
    }

    THEN("generating 10 steps should return 31 points") {
      auto ipath = path.generate(10);
      std::string str;
      REQUIRE(ipath().size() == 31);
    }
  }

  SUBCASE("automatic angle calculation") {

    GIVEN("a simple path") {
      MockQuinticPath path({{0_in, 0_in}, {0_in, 1_in}, {0_in, 2_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path()[0]->theta == 0_deg);
        CHECK(path.path()[1]->theta == 0_deg);
        CHECK(path.path()[2]->theta == 0_deg);
      }
    }

    GIVEN("a sideways path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 0_in}, {2_in, 0_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path()[0]->theta == 90_deg);
        CHECK(path.path()[1]->theta == 90_deg);
        CHECK(path.path()[2]->theta == 90_deg);
      }
    }

    GIVEN("an angled path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 1_in}, {2_in, 2_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path()[0]->theta == 45_deg);
        CHECK(path.path()[1]->theta == 45_deg);
        CHECK(path.path()[2]->theta == 45_deg);
      }
    }

    GIVEN("a curved path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 0_in}, {1_in, 1_in}, {2_in, 0_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path()[0]->theta == 90_deg);
        CHECK(path.path()[1]->theta == 45_deg);
        CHECK(path.path()[2]->theta == 90_deg);
        CHECK(path.path()[3]->theta == 135_deg);
      }
    }
  }
}