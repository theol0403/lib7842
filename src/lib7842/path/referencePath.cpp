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

std::unique_ptr<AbstractPath> ReferencePath::copyPtr() const {
  return std::make_unique<ReferencePath>(*this);
}

} // namespace lib7842