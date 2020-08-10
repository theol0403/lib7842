#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/pathStepper.hpp"

TEST_CASE("PathStepper") {
  auto i = PathStepper(Line({0_m, 0_m}, {0_m, 1_m}), StepBy::Count(1));
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
  auto v = Line({0_m, 0_m}, {0_m, 1_m}).generate(StepBy::Count(100));
  std::cout << v.size() << std::endl;
}