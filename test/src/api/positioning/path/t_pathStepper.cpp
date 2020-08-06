#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

TEST_CASE("PathStepper") {
  auto i = PathStepper(Line({0_m, 0_m}, {1_m, 1_m}), StepBy::Count(100));

  for (auto&& point : i) {
    std::cout << point << std::endl;
  }
}