#include "simplePath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::vector<QPoint>& ipath) : path(ipath) {}

std::vector<QPoint>& SimplePath::get() {
  return path;
}

SimplePath SimplePath::extract() const {
  return SimplePath(path);
}

ReferencePath SimplePath::extractRef() const {
  return ReferencePath({path.begin(), path.end()});
}

} // namespace lib7842