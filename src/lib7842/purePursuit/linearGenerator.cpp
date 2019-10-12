#include "linearGenerator.hpp"

namespace lib7842 {

SimplePath LinearGenerator::generate(
  const AbstractPath& ipath, const QLength& ispacing, const smoothParams_t& ismoothParams) {
  return smoothen(insert(ipath, ispacing), ismoothParams);
}

SimplePath LinearGenerator::insert(const AbstractPath& ipath, const QLength& ispacing) {
  ReferencePath srcPath = ipath.extractRef();
  SimplePath destPath;

  for (size_t i = 0; i < srcPath().size() - 1; i++) {
    const Vector& start = srcPath()[i].get();
    const Vector& end = srcPath()[i + 1].get();
    Vector diff = end - start;

    // number of points needed
    size_t numInsert = std::ceil((Vector::mag(diff) / ispacing).convert(number));
    // reserve vector capacity
    destPath().reserve(destPath().capacity() + numInsert);
    // how much to increment each point
    Vector step = diff / numInsert;

    for (size_t j = 0; j < numInsert; j++) {
      destPath().emplace_back(start + (step * j));
    }
  }

  // push the last point
  if (srcPath().size() > 0) destPath().emplace_back(srcPath().back());
  return destPath;
}

SimplePath
  LinearGenerator::smoothen(const AbstractPath& ipath, const smoothParams_t& ismoothParams) {
  ReferencePath srcPath = ipath.extractRef();
  SimplePath destPath = ipath.extract();

  double weight = 1.0 - ismoothParams.weight;
  QLength change = ismoothParams.tolerance;
  while (change >= ismoothParams.tolerance) {
    change = 0.0_in;
    for (size_t i = 1; i < srcPath().size() - 1; i++) {
      for (size_t j = 0; j < 2; j++) {
        QLength& destPoint = destPath()[i].at(j);
        QLength dataFac = ismoothParams.weight * (srcPath()[i].get().read(j) - destPoint);
        QLength smoothFac =
          weight * (destPath()[i - 1].read(j) + srcPath()[i + 1].get().read(j) - (2.0 * destPoint));
        destPoint += (dataFac + smoothFac);
        change = (destPoint - destPath()[i].read(j)).abs();
      }
    }
  }
  return destPath;
}

} // namespace lib7842