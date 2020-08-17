#include "lib7842/api/positioning/path/piecewise.hpp"
#include "lib7842/test/test.hpp"
#include "line.hpp"
namespace test {
TEST_CASE("Piecewise") {
  auto p = make_piecewise<Line>({{{0_m, 0_m}, {1_m, 1_m}}, {{0_m, 0_m}, {1_m, 1_m}}})
             .step(StepBy::T(0.01));
}
} // namespace test