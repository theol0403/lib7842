#include "lib7842/api/positioning/path/stepper.hpp"
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/point/vector.hpp"

#include "lib7842/test/test.hpp"
namespace test {
class DontCopy : public PathHelper<DontCopy> {
public:
  constexpr DontCopy() = default;
  constexpr DontCopy(const DontCopy&) = delete;
  constexpr DontCopy(DontCopy&&) = default;
  constexpr State calc(double /*t*/) const override { return {}; };
  constexpr QCurvature curvature(double /*t*/) const override { return 0 / meter; }
};

consteval auto f() {
  return Stepper(Line({0_m, 0_m}, {1_m, 1_m}), StepBy::ConstCount<100>()).generate();
}

consteval auto f2() { return Line({0_m, 0_m}, {1_m, 1_m}).generate(StepBy::ConstCount<100>()); }

consteval auto f3() {
  return Line({0_m, 0_m}, {1_m, 1_m}).step(StepBy::ConstCount<100>()).generate();
}

TEST_CASE("Stepper") {

  SUBCASE("Lvalue") {
    auto p = DontCopy();
    auto i = Stepper(p, StepBy::Count(100));
    for (auto&& point : i) {
      REQUIRE(point == State());
    }
  }

  SUBCASE("Rvalue") {
    auto i = Stepper(DontCopy(), StepBy::Count(100));
    for (auto&& point : i) {
      REQUIRE(point == State());
    }
  }

  SUBCASE("Steppers") {
    SUBCASE("Count") {
      auto i = Stepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Count(100));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j) == Vector(0_m, j / 100.0 * meter));
      }
    }
    SUBCASE("T") {
      auto i = Stepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::T(0.01));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j) == Vector(0_m, j / 100.0 * meter));
      }
    }
    SUBCASE("Dist") {
      auto i = Stepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Dist(0.01_m));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j).y.convert(meter) == Approx(j / 100.0));
      }
    }

    SUBCASE("Step") {
      SUBCASE("Lvalue") {
        DontCopy l;
        auto s = l.step(StepBy::T(0.01));
        for (auto&& point : s) {
          REQUIRE(point == State());
        }
      }

      SUBCASE("Rvalue") {
        auto s = DontCopy().step(StepBy::T(0.01));
        for (auto&& point : s) {
          REQUIRE(point == State());
        }
      }
    }
  }

  SUBCASE("Compile Time") {
    auto v = f();
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v.at(i) == State(i / 100.0 * meter, i / 100.0 * meter, 45_deg));
    }
  }

  SUBCASE("Compile Time Generate") {
    auto v = f2();
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v.at(i) == State(i / 100.0 * meter, i / 100.0 * meter, 45_deg));
    }
  }

  SUBCASE("Compile Time Step") {
    auto v = f3();
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v.at(i) == State(i / 100.0 * meter, i / 100.0 * meter, 45_deg));
    }
  }
}
} // namespace test
