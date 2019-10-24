#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const std::shared_ptr<AbstractPath>& ipath) {
  addPath(std::move(ipath));
}

CompoundPath& CompoundPath::addPath(const std::shared_ptr<AbstractPath>& ipath) {
  paths.emplace_back(std::move(ipath));
  return *this;
}

CompoundPath& CompoundPath::copyPath(const AbstractPath& ipath) {
  addPath(std::move(ipath.copyPtr()));
  return *this;
}

CompoundPath& CompoundPath::importPath(const AbstractPath& ipath) {
  addPath(std::move(ipath.movePtr()));
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

/**
 * Return shared pointer to copy of path
 */
std::shared_ptr<AbstractPath> CompoundPath::copyPtr() const {
  return std::make_shared<CompoundPath>(*this);
}

/**
 * Move the path into a shared pointer and return pointer
 */
std::shared_ptr<AbstractPath> CompoundPath::movePtr() const {
  return std::make_shared<CompoundPath>(std::move(*this));
}

} // namespace lib7842