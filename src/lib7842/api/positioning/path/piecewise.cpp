#include "lib7842/api/positioning/path/piecewise.hpp"
#include "lib7842/test/test.hpp"
#include "line.hpp"
namespace test {
TEST_CASE("Piecewise") {
  SUBCASE("Explicit") {
    auto l = Piecewise<Line, 2>({{{0_m, 0_m}, {1_m, 1_m}}, {{1_m, 1_m}, {2_m, 2_m}}})
               .step(StepBy::Count(5))
               .generate();
    REQUIRE(l.size() == 6);
    for (size_t i = 0; i < l.size(); ++i) {
      REQUIRE(l[i] == State(i / 2.5 * meter, i / 2.5 * meter, 45_deg));
    }
  }

  SUBCASE("Deduction") {
    auto l = Piecewise({Line {{0_m, 0_m}, {1_m, 1_m}}, Line {{1_m, 1_m}, {2_m, 2_m}}})
               .step(StepBy::Count(5))
               .generate();
    REQUIRE(l.size() == 6);
    for (size_t i = 0; i < l.size(); ++i) {
      REQUIRE(l[i] == State(i / 2.5 * meter, i / 2.5 * meter, 45_deg));
    }
  }

  SUBCASE("Helper") {
    auto l = make_piecewise<Line>({{{0_m, 0_m}, {1_m, 1_m}}, {{1_m, 1_m}, {2_m, 2_m}}})
               .step(StepBy::Count(5))
               .generate();
    REQUIRE(l.size() == 6);
    for (size_t i = 0; i < l.size(); ++i) {
      REQUIRE(l[i] == State(i / 2.5 * meter, i / 2.5 * meter, 45_deg));
    }
  }
}
} // namespace test