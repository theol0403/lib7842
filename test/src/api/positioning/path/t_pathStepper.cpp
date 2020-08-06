#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

TEST_CASE("PathStepper") {
  auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Dist(0.5_m));

  for (auto&& point : i) {
    std::cout << point << std::endl;
  }
  std::vector<State> d;
  std::move(i.begin(), i.end(), std::back_inserter(d));
}