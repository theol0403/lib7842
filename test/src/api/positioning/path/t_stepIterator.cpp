

#include "../test/include/test.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/path/stepIterator.hpp"

TEST_CASE("QuinticSegment") {
  Line l({0_in, 0_in}, {0_in, 0_in});

  auto i = StepIterator<>::create(l, [](double, auto& path) { return 0.01; });
}