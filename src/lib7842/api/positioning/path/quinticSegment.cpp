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

  double xStartSlope = startSlope * std::sin(start.theta.convert(radian));
  double yStartSlope = startSlope * std::cos(start.theta.convert(radian));
  double xEndSlope = endSlope * std::sin(end.theta.convert(radian));
  double yEndSlope = endSlope * std::cos(end.theta.convert(radian));

  QuinticPolynomial xPoly(start.x.convert(meter), xStartSlope, end.x.convert(meter), xEndSlope);
  QuinticPolynomial yPoly(start.y.convert(meter), yStartSlope, end.y.convert(meter), yEndSlope);

  SimplePath temp;
  temp().reserve(isteps);

  for (size_t i = 0; i <= (iend ? isteps : isteps - 1); i++) {
    temp().emplace_back(std::make_shared<Vector>(xPoly.calculate(i / (double)isteps) * meter,
                                                 yPoly.calculate(i / (double)isteps) * meter));
  }

  return temp;
}

} // namespace lib7842