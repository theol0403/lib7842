#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

class test {
public:
  constexpr test() = default;
  constexpr State calc(double) const { return {}; }
};

consteval auto f() { return PathStepper(test(), StepBy::T(0.01)); }

TEST_CASE("PathStepper") {
  // constexpr auto i = PathStepper(test(), StepBy::T(0.01));
  auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Count(100));

  auto v = f();

  for (auto&& point : i) {
    std::cout << point << std::endl;
  }
  std::vector<State> d;
  std::move(i.begin(), i.end(), std::back_inserter(d));
  std::cout << d.size() << std::endl;
}