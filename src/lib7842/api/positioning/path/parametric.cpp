#include "lib7842/api/positioning/path/parametric.hpp"
#include "lib7842/test/test.hpp"
namespace test {

TEST_CASE("Parametric") {
  Parametric<CubicHermite> c({0_in, 0_in, 0_deg}, {1_in, 1_in, 0_deg});

  Parametric s({{0_m, 0_m}, {1_m, 1_m}, {2_m, 2_m}, {3_m, 3_m}});
}
} // namespace test
