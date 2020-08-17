#include "lib7842/api/positioning/path/piecewise.hpp"
#include "lib7842/test/test.hpp"
#include "line.hpp"
namespace test {
TEST_CASE("Piecewise") {
  Piecewise<Line, 2> l({{{0_m, 0_m}, {1_m, 1_m}}, {{0_m, 0_m}, {1_m, 1_m}}});
  // auto f = &Path::calc;
}
} // namespace test