#pragma once

namespace lib7842 {

class QuinticPolynomial {
public:
  QuinticPolynomial(double istart, double istartSlope, double iend, double iendSlope);

  double calculate(double t);

protected:
  double start {};
  double startSlope {};
  double end {};
  double endSlope {};
};

} // namespace lib7842