#include "quinticSegment.hpp"
#include "lib7842/api/other/quinticPolynomial.hpp"

namespace lib7842 {

QuinticSegment::QuinticSegment(const DataState& istart, const DataState& iend) :
  start(istart), end(iend) {}

SimplePath QuinticSegment::generate(int isteps) const {

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
  for (double t = 0.0; t <= 1.0; t += dt) {
    temp().emplace_back(
      std::make_shared<Vector>(xPoly.calculate(t) * meter, yPoly.calculate(t) * meter));
  }

  return temp;
}

} // namespace lib7842