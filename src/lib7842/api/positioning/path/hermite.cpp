#include "lib7842/api/positioning/path/hermite.hpp"
#include "lib7842/test/test.hpp"
#include <iostream>
namespace test {

TEST_CASE("Hermite") { CubicHermite(0, 1, 0, 0); }
} // namespace test
