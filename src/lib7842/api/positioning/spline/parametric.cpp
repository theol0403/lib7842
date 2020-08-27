#include "lib7842/api/positioning/spline/bezier.hpp"
#include "lib7842/api/positioning/spline/hermite.hpp"
#include "lib7842/test/test.hpp"
namespace test {

TEST_CASE("Parametric") {
  CubicHermite c({0_in, 0_in, 0_deg}, {1_in, 1_in, 0_deg});
  CubicBezier s({{0_m, 0_m}, {1_m, 1_m}, {2_m, 2_m}, {3_m, 3_m}});
}
} // namespace test
