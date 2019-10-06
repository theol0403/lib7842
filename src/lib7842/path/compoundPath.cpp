#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath& CompoundPath::addPoint(const QPoint& ipoint) {
  path.push_back(ipoint);
  return *this;
}

CompoundPath& CompoundPath::addSegment(const AbstractPath& isegment) {
  path.push_back(isegment);
  return *this;
}

CompoundPath& CompoundPath::addPoints(const std::vector<QPoint>& ipoints) {
  for (auto&& ipoint : ipoints) {
    addPoint(ipoint);
  }
  return *this;
}

CompoundPath& CompoundPath::addSegments(const std::vector<AbstractPath>& isegments) {
  for (auto&& isegment : isegments) {
    addSegment(isegment);
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
      auto&& points = std::get<AbstractPathRef>(segment).get().extractRef();
      for (auto&& point : points) {
        temp.push_back(point);
      }
    }
  }

  return temp;
}

} // namespace lib7842