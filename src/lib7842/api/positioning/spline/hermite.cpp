#include "lib7842/api/positioning/spline/hermite.hpp"
#include "lib7842/test/test.hpp"
namespace test {
TEST_CASE("Hermite") { HermiteFnc<3>(0, 1, 0, 0); }
} // namespace test
