#include "lib7842/api/positioning/path/quinticSegment.hpp"
#include <utility>

namespace lib7842 {

QuinticPolynomial::QuinticPolynomial(double istart, double istartSlope, double iend,
                                     double iendSlope) :
  start(istart), startSlope(istartSlope), end(iend), endSlope(iendSlope) {}

double QuinticPolynomial::calculate(double t) const {
  double u = end - start - startSlope;
  double v = endSlope - startSlope;

  double a3 = 10 * u - 4 * v;
  double a4 = -15 * u + 7 * v;
  double a5 = 6 * u - 3 * v;

  double coeffs[] = {start, startSlope, 0, a3, a4, a5};

  double xt = 0;
  for (size_t power = 0; power < 6; power++) {
    xt += coeffs[power] * std::pow(t, power);
  }

  return xt;
}

QuinticSegment::QuinticSegment(DataState istart, DataState iend) :
  start(std::move(istart)), end(std::move(iend)) {}

SimplePath QuinticSegment::generate(int isteps, bool iend) const {
  auto startSlope = start.getData<double>("slope");
  auto endSlope = end.getData<double>("slope");

  double xStartSlope = startSlope * std::sin(start.theta.convert(radian));
  double yStartSlope = startSlope * std::cos(start.theta.convert(radian));
  double xEndSlope = endSlope * std::sin(end.theta.convert(radian));
  double yEndSlope = endSlope * std::cos(end.theta.convert(radian));

  QuinticPolynomial xPoly(start.x.convert(meter), xStartSlope, end.x.convert(meter), xEndSlope);
  QuinticPolynomial yPoly(start.y.convert(meter), yStartSlope, end.y.convert(meter), yEndSlope);

  SimplePath temp;
  temp().reserve(isteps);

  for (size_t i = 0; i <= (iend ? isteps : isteps - 1); i++) {
    temp().emplace_back(
      std::make_shared<Vector>(xPoly.calculate(i / static_cast<double>(isteps)) * meter,
                               yPoly.calculate(i / static_cast<double>(isteps)) * meter));
  }

  return temp;
}

} // namespace lib7842

#include "lib7842/test.hpp"
namespace test {
TEST_CASE("QuinticSegment") {

  GIVEN("a simple segment") {
    DataState start {0_in, 0_in, 0_deg};
    start.setData("slope", 1.0);
    DataState end {0_in, 1_in, 0_deg};
    end.setData("slope", 1.0);
    QuinticSegment segment(start, end);

    THEN("generating one step should return both points") {
      auto path = segment.generate(1);
      REQUIRE(path().size() == 2);
      CHECK(*path().at(0) == start);
      CHECK(path().at(1)->y.convert(inch) == Approx(1));
    }

    THEN("generating two steps should return interpolated points") {
      auto path = segment.generate(2);
      REQUIRE(path().size() == 3);
      CHECK(*path().at(0) == start);
      CHECK(path().at(1)->y.convert(inch) == Approx(0.5));
      CHECK(path().at(2)->y.convert(inch) == Approx(1));
    }
  }

  GIVEN("an s curve") {
    DataState start {0_in, 0_in, 90_deg};
    start.setData("slope", 1.0);
    DataState end {2_in, 4_in, 90_deg};
    end.setData("slope", 1.0);
    QuinticSegment segment(start, end);

    THEN("generating one step should return both points") {
      auto path = segment.generate(1);
      REQUIRE(path().size() == 2);
      CHECK(*path().at(0) == start);
      CHECK(path().at(1)->x.convert(inch) == Approx(2));
      CHECK(path().at(1)->y.convert(inch) == Approx(4));
    }

    THEN("generating two steps should return interpolated points") {
      auto path = segment.generate(2);
      REQUIRE(path().size() == 3);
      CHECK(*path().at(0) == start);
      CHECK(path().at(1)->x.convert(inch) == Approx(1));
      CHECK(path().at(1)->y.convert(inch) == Approx(2));
      CHECK(path().at(2)->x.convert(inch) == Approx(2));
      CHECK(path().at(2)->y.convert(inch) == Approx(4));
    }
  }
}
} // namespace test
