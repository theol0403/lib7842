#include "lib7842/api/positioning/spline/hermite.hpp"
#include "lib7842/test/test.hpp"
namespace test {
TEST_CASE("Hermite") {
  HermiteFnc<3>(0, 1, 0, 0);
  auto l = make_piecewise<CubicHermite>(
    {{{0_m, 0_m, 0_deg}, {1_m, 1_m, 0_deg}}, {{1_m, 1_m, 0_deg}, {2_m, 2_m, 0_deg}}});
}
} // namespace test
