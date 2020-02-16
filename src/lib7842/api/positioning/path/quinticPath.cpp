#include "quinticPath.hpp"
#include "compoundPath.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "quinticSegment.hpp"

namespace lib7842 {

QuinticPath::QuinticPath(const StatePath& ipath, double islopeScalar) :
  path(ipath), slopeScalar(islopeScalar) {}

QuinticPath::QuinticPath(const SimplePath& ipath, double islopeScalar) :
  path(ipath), slopeScalar(islopeScalar) {
  if (path().size() > 1) {
    path()[0]->theta = State(*path()[0]).angleTo(*path()[1]);
    for (size_t i = 1; i < path().size() - 1; i++) {
      path()[i]->theta = State(*path()[i - 1]).angleTo(*path()[i + 1]);
    }
    path().back()->theta = State(*path()[path().size() - 2]).angleTo(*path().back());
  }
}

using DataStatePath = DiscretePath<DataState>;

SimplePath QuinticPath::generate(int isteps, bool iend) const {
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

  return compound.generate(isteps, iend);
}

} // namespace lib7842