#include "lib7842/api/positioning/path/bezier.hpp"
#include "lib7842/test/test.hpp"
#include <iostream>
namespace test {

TEST_CASE("Bezier") {
  std::array a {0.0, 1.0, 2.0, 3.0};
  Bezier b(a);
}
} // namespace test
