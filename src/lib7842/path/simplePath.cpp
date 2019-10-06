#include "simplePath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::vector<QPoint>& ipath) : path(ipath) {}

std::vector<QPoint>& SimplePath::get() {
  return path;
}

// SimplePath SimplePath::extract() const {}
// ReferencePath SimplePath::extractRef() const {}

} // namespace lib7842