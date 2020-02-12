#include "quinticPolynomial.hpp"
#include <cmath>
#include <cstddef>

namespace lib7842 {

QuinticPolynomial::QuinticPolynomial(double istart, double istartSlope, double iend,
                                     double iendSlope) :
  start(istart), startSlope(istartSlope), end(iend), endSlope(iendSlope) {}

double QuinticPolynomial::calculate(double t) {
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
} // namespace lib7842