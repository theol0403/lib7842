#include "lib7842/api/positioning/path/parametric.hpp"
#include "lib7842/test/test.hpp"
namespace test {
class Test {
public:
  constexpr Test() = default;
  virtual ~Test() = default;

  constexpr virtual double calc(double x) const { return 0; }
  constexpr virtual double calc_d(double x) const { return 0; }
  constexpr virtual double calc_d2(double x) const { return 0; }
};

TEST_CASE("Parametric") {
  Parametric t((Test()), Test());
  t.calc(0.0);
}
} // namespace test
