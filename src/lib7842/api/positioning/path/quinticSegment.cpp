#include "quinticSegment.hpp"
#include "lib7842/api/other/quinticPolynomial.hpp"

namespace lib7842 {

QuinticSegment::QuinticSegment(const DataState& istart, const DataState& iend) :
  start(istart), end(iend) {}

SimplePath QuinticSegment::generate(int isteps) const {

  double startI = start.getData<double>("importance");
  double endI = end.getData<double>("importance");

  double startIx = startI * std::cos(start.theta.convert(radian));
  double startIy = startI * std::sin(start.theta.convert(radian));
  double endIx = endI * std::cos(end.theta.convert(radian));
  double endIy = endI * std::sin(end.theta.convert(radian));

  QuinticPolynomial xPolynomial(start.x.convert(meter), startIx, end.x.convert(meter), endIx);
  QuinticPolynomial yPolynomial(start.y.convert(meter), startIy, end.y.convert(meter), endIy);

  double dt = 1.0 / isteps;

  SimplePath temp;
  temp().reserve(isteps);

  for (double t = 0.0; t <= 1.0; t += dt) {
    temp().emplace_back(
      std::make_shared<Vector>(xPolynomial.calculate(t) * meter, yPolynomial.calculate(t) * meter));
  }

  return temp;
}

} // namespace lib7842