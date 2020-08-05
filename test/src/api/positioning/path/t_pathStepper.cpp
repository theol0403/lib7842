#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

TEST_CASE("PathStepper") {
  Line l({0_m, 0_m}, {1_m, 1_m});

  auto i = PathStepper(l, [](double, auto& path) { return 0.1; });

  for (auto&& point : i) {
    std::cout << point << std::endl;
  }
}