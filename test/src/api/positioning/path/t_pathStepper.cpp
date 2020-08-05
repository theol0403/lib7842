#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

TEST_CASE("PathStepper") {
  Line l({0_in, 0_in}, {1_in, 1_in});

  auto i = PathStepper(l, [](double, auto& path) { return 0.01; });

  for (auto&& point : i) {
    std::cout << point << std::endl;
  }
}