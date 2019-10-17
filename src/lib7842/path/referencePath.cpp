#include "referencePath.hpp"

namespace lib7842 {

ReferencePath::ReferencePath(const std::vector<VectorRef>& ipath) : path(ipath) {}

std::vector<VectorRef>& ReferencePath::get() {
  return path;
}

std::vector<VectorRef>& ReferencePath::operator()() {
  return path;
}

SimplePath ReferencePath::generate(const size_t isteps) const {
  return SimplePath(std::vector<Vector> {path.begin(), path.end()}).generate(isteps);
}

std::shared_ptr<AbstractPath> ReferencePath::copyPtr() const {
  return std::make_shared<ReferencePath>(*this);
}

std::shared_ptr<AbstractPath> ReferencePath::movePtr() const {
  return std::make_shared<ReferencePath>(std::move(*this));
}

} // namespace lib7842