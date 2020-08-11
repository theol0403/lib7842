#include "lib7842/api/positioning/path/quinticPath.hpp"
#include "lib7842/api/positioning/path/compoundPath.hpp"
#include "lib7842/api/positioning/path/quinticSegment.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include <utility>

namespace lib7842 {

QuinticPath::QuinticPath(StatePath ipath, double islopeScalar) :
  path(std::move(ipath)), slopeScalar(islopeScalar) {}

QuinticPath::QuinticPath(const SimplePath& ipath, double islopeScalar) :
  path(ipath), slopeScalar(islopeScalar) {
  if (path().size() > 1) {
    path()[0]->theta = Vector::angle(*path()[0], *path()[1]);
    for (size_t i = 1; i < path().size() - 1; i++) {
      path()[i]->theta = Vector::angle(*path()[i - 1], *path()[i + 1]);
    }
    path().back()->theta = Vector::angle(*path()[path().size() - 2], *path().back());
  }
}

using DataStatePath = DiscretePath<DataState>;

SimplePath QuinticPath::generate(int isteps, bool iend) const {
  DataStatePath temp(path);

  for (size_t i = 0; i < temp().size() - 1; i++) {
    auto& p1 = temp()[i];
    auto& p2 = temp()[i + 1];
    double slope = slopeScalar * MathPoint::dist(*p1, *p2);
    p1->setData("slope", slope);
    if (i == temp().size() - 2) p2->setData("slope", slope);
  }

  CompoundPath compound;

  for (size_t i = 0; i < temp().size() - 1; i++) {
    auto& p1 = temp()[i];
    auto& p2 = temp()[i + 1];
    compound.add(QuinticSegment(*p1, *p2));
  }

  return compound.generate(isteps, iend);
}

} // namespace lib7842

#include "lib7842/test.hpp"
namespace test {
class MockQuinticPath : public QuinticPath {
public:
  using QuinticPath::QuinticPath;
  using QuinticPath::path;
};

TEST_CASE("QuinticPath") {

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
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->y.convert(inch) == Approx(1));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating two steps should return interpolated points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->y.convert(inch) == Approx(0.5));
        CHECK(ipath().at(2)->y.convert(inch) == Approx(1));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating 10 steps should return interpolated points") {
        auto ipath = path.generate(10);
        REQUIRE(ipath().size() == 11);

        for (size_t i = 0; i < ipath().size(); i++) {
          CHECK(ipath().at(i)->y.convert(inch) == Approx(i * 0.1));
          CHECK(ipath().at(i)->x.convert(inch) == Approx(0));
        }
      }
    }

    GIVEN("an s curve") {
      State start {0_in, 0_in, 90_deg};
      QuinticPath path({start, {2_in, 4_in, 90_deg}}, 1);

      THEN("generating one step should return both points") {
        auto ipath = path.generate(1);
        REQUIRE(ipath().size() == 2);
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->x.convert(inch) == Approx(2));
        CHECK(ipath().at(1)->y.convert(inch) == Approx(4));
      }

      THEN("generating two steps should return interpolated points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 3);
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->x.convert(inch) == Approx(1));
        CHECK(ipath().at(1)->y.convert(inch) == Approx(2));
        CHECK(ipath().at(2)->x.convert(inch) == Approx(2));
        CHECK(ipath().at(2)->y.convert(inch) == Approx(4));
      }

      THEN("generating 4 steps should return interpolated points") {
        auto ipath = path.generate(4);
        REQUIRE(ipath().size() == 5);
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(2)->x.convert(inch) == Approx(1));
        CHECK(ipath().at(2)->y.convert(inch) == Approx(2));
        CHECK(ipath().at(4)->x.convert(inch) == Approx(2));
        CHECK(ipath().at(4)->y.convert(inch) == Approx(4));
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
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->y.convert(inch) == Approx(1));
        CHECK(ipath().at(2)->y.convert(inch) == Approx(2));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating two steps should return 5 points") {
        auto ipath = path.generate(2);
        REQUIRE(ipath().size() == 5);
        CHECK(*ipath().at(0) == start);
        CHECK(ipath().at(1)->y.convert(inch) == Approx(0.5));
        CHECK(ipath().at(2)->y.convert(inch) == Approx(1));
        CHECK(ipath().at(3)->y.convert(inch) == Approx(1.5));
        CHECK(ipath().at(4)->y.convert(inch) == Approx(2));

        for (auto&& point : ipath()) {
          CHECK(point->x.convert(inch) == Approx(0));
        }
      }

      THEN("generating 10 steps should return interpolated points") {
        auto ipath = path.generate(10);
        REQUIRE(ipath().size() == 21);

        for (size_t i = 0; i < ipath().size(); i++) {
          CHECK(ipath().at(i)->y.convert(inch) == Approx(i * 0.1));
          CHECK(ipath().at(i)->x.convert(inch) == Approx(0));
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
      REQUIRE(ipath().size() == 31);
    }
  }

  SUBCASE("automatic angle calculation") {

    GIVEN("a simple path") {
      MockQuinticPath path({{0_in, 0_in}, {0_in, 1_in}, {0_in, 2_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path().at(0)->theta == 0_deg);
        CHECK(path.path().at(1)->theta == 0_deg);
        CHECK(path.path().at(2)->theta == 0_deg);
      }
    }

    GIVEN("a sideways path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 0_in}, {2_in, 0_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path().at(0)->theta == 90_deg);
        CHECK(path.path().at(1)->theta == 90_deg);
        CHECK(path.path().at(2)->theta == 90_deg);
      }
    }

    GIVEN("an angled path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 1_in}, {2_in, 2_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path().at(0)->theta == 45_deg);
        CHECK(path.path().at(1)->theta == 45_deg);
        CHECK(path.path().at(2)->theta == 45_deg);
      }

      THEN("generating should work") {
        auto ipath = path.generate(10);
        REQUIRE(ipath().size() == 21);
        for (size_t i = 0; i < ipath().size(); i++) {
          CHECK(ipath().at(i)->y.convert(inch) == Approx(i * 0.1));
          CHECK(ipath().at(i)->x.convert(inch) == Approx(i * 0.1));
        }
      }
    }

    GIVEN("a curved path") {
      MockQuinticPath path({{0_in, 0_in}, {1_in, 0_in}, {1_in, 1_in}, {2_in, 0_in}}, 1);

      THEN("the points should be facing the proper way") {
        CHECK(path.path().at(0)->theta == 90_deg);
        CHECK(path.path().at(1)->theta == 45_deg);
        CHECK(path.path().at(2)->theta == 90_deg);
        CHECK(path.path().at(3)->theta == 135_deg);
      }
    }
  }
}
} // namespace test