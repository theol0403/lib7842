#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>

consteval auto d() { return PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Dist(0.1_m)); }

TEST_CASE("PathStepper") {
  auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Dist(0.1_m));
  auto v = i.generate();
  std::cout << v.size() << std::endl;
  for (auto&& point : i) {
    std::cout << point << std::endl;
  }

  std::vector<State> d;
  std::move(i.begin(), i.end(), std::back_inserter(d));
  std::cout << d.size() << std::endl;
}

TEST_CASE("Generate") {
  // auto v = Line({0_m, 0_m}, {0_m, 1_m}).generate(StepBy::Count(100));
  // std::cout << v.size() << std::endl;
}

class DontCopy {
public:
  DontCopy() = default;
  DontCopy(const DontCopy&) = delete;
  DontCopy(DontCopy&&) = default;
  State calc(double /*t*/) const { return {}; };
};

TEST_CASE("PathStepper") {

  SUBCASE("Lvalue") {
    auto p = DontCopy();
    auto i = PathStepper(p, StepBy::Count(100));
    for (auto&& point : i) {
      REQUIRE(point == State());
    }
  }

  SUBCASE("Rvalue") {
    auto i = PathStepper(DontCopy(), StepBy::Count(100));
    for (auto&& point : i) {
      REQUIRE(point == State());
    }
  }

  SUBCASE("Steppers") {
    SUBCASE("Count") {
      auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Count(100));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j) == Vector(0_m, j / 100.0 * meter));
      }
    }
    SUBCASE("T") {
      auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::T(0.01));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j) == Vector(0_m, j / 100.0 * meter));
      }
    }
    SUBCASE("Dist") {
      auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Dist(0.01_m));
      auto v = i.generate();
      REQUIRE(v.size() == 101);
      for (size_t j = 0; j < v.size(); ++j) {
        REQUIRE(v.at(j).y.convert(meter) == Approx(j / 100.0));
      }
    }
  }
}