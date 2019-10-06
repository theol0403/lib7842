#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const AbstractPath& ipath) {
  addPath(ipath);
}

CompoundPath& CompoundPath::addPoint(const QPoint& ipoint) {
  path.push_back(ipoint);
  return *this;
}

CompoundPath& CompoundPath::addPath(const AbstractPath& ipath) {
  path.push_back(ipath);
  return *this;
}

CompoundPath& CompoundPath::addPoints(const std::vector<QPoint>& ipoints) {
  for (auto&& ipoint : ipoints) {
    addPoint(ipoint);
  }
  return *this;
}

CompoundPath& CompoundPath::addPaths(const std::vector<AbstractPathRef>& ipaths) {
  for (auto&& ipath : ipaths) {
    addPath(ipath);
  }
  return *this;
}

SimplePath CompoundPath::extract() const {
  ReferencePath temp = extractRef();
  return SimplePath({temp.get().begin(), temp.get().end()});
}

ReferencePath CompoundPath::extractRef() const {
  std::vector<ReferencePath::PointRef> temp;
  for (auto&& segment : path) {
    if (std::holds_alternative<QPoint>(segment)) {
      temp.push_back(std::get<QPoint>(segment));
    } else {
      auto&& ipath = std::get<AbstractPathRef>(segment).get().extractRef();
      for (auto&& ipoint : ipath.get()) {
        temp.push_back(ipoint);
      }
    }
  }

  return ReferencePath(temp);
}

} // namespace lib7842