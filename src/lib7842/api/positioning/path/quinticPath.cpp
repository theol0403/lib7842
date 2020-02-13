#include "quinticPath.hpp"
#include "compoundPath.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "quinticSegment.hpp"

namespace lib7842 {

QuinticPath::QuinticPath(const StatePath& ipath, double islopeScalar) :
  path(ipath), slopeScalar(islopeScalar) {}

using DataStatePath = DiscretePath<DataState>;

SimplePath QuinticPath::generate(int isteps) const {
  DataStatePath temp(path);

  for (size_t i = 0; i < temp().size() - 1; i++) {
    auto& p1 = temp()[i];
    auto& p2 = temp()[i + 1];

    double slope = slopeScalar * MathPoint::dist(*p1, *p2);
    p1->setData("slope", slope);
    if (i == temp().size() - 2) p2->setData("slope", slope);
  }

  CompoundPath compound;

  for (size_t i = 0; i < temp().size() - 1; i++) {
    auto& p1 = temp()[i];
    auto& p2 = temp()[i + 1];
    compound.add(QuinticSegment(*p1, *p2));
  }

  return compound.generate(isteps);
}

} // namespace lib7842