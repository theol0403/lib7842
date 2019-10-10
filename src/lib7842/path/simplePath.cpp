#include "simplePath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::initializer_list<QPoint>& ipath) : path(ipath) {}
SimplePath::SimplePath(const std::vector<QPoint>& ipath) : path(ipath) {}

std::vector<QPoint>& SimplePath::get() {
  return path;
}

std::vector<QPoint>& SimplePath::operator()() {
  return path;
}

SimplePath SimplePath::extract() const {
  return *this;
}

ReferencePath SimplePath::extractRef() const {
  return ReferencePath({path.begin(), path.end()});
}

std::shared_ptr<AbstractPath> SimplePath::copyPtr() const {
  return std::make_shared<SimplePath>(*this);
}

std::shared_ptr<AbstractPath> SimplePath::movePtr() const {
  return std::make_shared<SimplePath>(std::move(*this));
}

} // namespace lib7842