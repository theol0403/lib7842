#include "lib7842/api/positioning/path/piecewise.hpp"
#include "lib7842/test/test.hpp"
#include "line.hpp"
namespace test {
TEST_CASE("Piecewise") {
  Piecewise l({Line({0_m, 0_m}, {1_m, 1_m}), Line({0_m, 0_m}, {1_m, 1_m})});
}
} // namespace test