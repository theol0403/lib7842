#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const std::shared_ptr<AbstractPath>& ipath) {
  addPath(std::move(ipath));
}

CompoundPath& CompoundPath::addPath(const std::shared_ptr<AbstractPath>& ipath) {
  paths.emplace_back(std::move(ipath));
  return *this;
}

/**
 * Interpolate the path
 *
 * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
 * @return generated path
 */
SimplePath CompoundPath::generate(const int isteps) const {
  SimplePath temp;
  for (auto&& path : paths) {
    SimplePath ipath = path->generate(isteps);
    temp().reserve(temp().size() + ipath().size());
    for (auto&& point : ipath()) {
      temp().emplace_back(point);
    }
  }
  return temp;
}
} // namespace lib7842