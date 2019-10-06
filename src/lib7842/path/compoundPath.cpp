#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(std::unique_ptr<AbstractPath> ipath) {
  addPath(std::move(ipath));
}

CompoundPath& CompoundPath::addPoint(const QPoint& ipoint) {
  path.emplace_back(ipoint);
  return *this;
}

CompoundPath& CompoundPath::addPoints(const std::vector<QPoint>& ipoints) {
  for (auto&& ipoint : ipoints) {
    addPoint(ipoint);
  }
  return *this;
}

CompoundPath& CompoundPath::addPath(std::unique_ptr<AbstractPath> ipath) {
  path.emplace_back(std::move(ipath));
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
      temp.emplace_back(std::get<QPoint>(segment));
    } else {
      ReferencePath ipath = std::get<AbstractPathPtr>(segment).get()->extractRef();
      for (auto&& ipoint : ipath.get()) {
        temp.emplace_back(ipoint);
      }
    }
  }

  return ReferencePath(temp);
}

} // namespace lib7842