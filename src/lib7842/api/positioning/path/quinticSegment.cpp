#include "quinticSegment.hpp"

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

QuinticSegment::QuinticSegment(const DataState& istart, const DataState& iend) :
  start(istart), end(iend) {}

SimplePath QuinticSegment::generate(int isteps, bool iend) const {
  double startSlope = start.getData<double>("slope");
  double endSlope = end.getData<double>("slope");

  double xStartSlope = startSlope * std::cos(start.theta.convert(radian));
  double yStartSlope = startSlope * std::sin(start.theta.convert(radian));
  double xEndSlope = endSlope * std::cos(end.theta.convert(radian));
  double yEndSlope = endSlope * std::sin(end.theta.convert(radian));

  QuinticPolynomial xPoly(start.x.convert(meter), xStartSlope, end.x.convert(meter), xEndSlope);
  QuinticPolynomial yPoly(start.y.convert(meter), yStartSlope, end.y.convert(meter), yEndSlope);

  SimplePath temp;
  temp().reserve(isteps);

  double dt = 1.0 / isteps;
  for (double t = 0.0; (iend ? t <= 1.0 : t < 1.0 - std::numeric_limits<double>::epsilon());
       t += dt) {
    temp().emplace_back(
      std::make_shared<Vector>(xPoly.calculate(t) * meter, yPoly.calculate(t) * meter));
  }

  return temp;
}

} // namespace lib7842