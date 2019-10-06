#include "referencePath.hpp"

namespace lib7842 {

ReferencePath::ReferencePath(const std::vector<PointRef>& ipath) : path(ipath) {}

std::vector<ReferencePath::PointRef>& ReferencePath::get() {
  return path;
}

SimplePath ReferencePath::extract() const {
  return SimplePath({path.begin(), path.end()});
}

ReferencePath ReferencePath::extractRef() const {
  return *this;
}

} // namespace lib7842