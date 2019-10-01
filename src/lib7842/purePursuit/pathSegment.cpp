#include "pathSegment.hpp"

namespace lib7842 {

PathSegment& PathSegment::addPoint(const PathPoint& ipoint) {
  segments.push_back(ipoint);
  return *this;
}

PathSegment& PathSegment::addSegment(const PathSegment& isegment) {
  segments.push_back(isegment);
  return *this;
}

PathSegment& PathSegment::addPoints(const std::vector<PathPoint>& ipoints) {
  for (auto&& ipoint : ipoints) {
    addPoint(ipoint);
  }
  return *this;
}

PathSegment& PathSegment::addSegments(const std::vector<PathSegment>& isegments) {
  for (auto&& isegment : isegments) {
    addSegment(isegment);
  }
  return *this;
}

std::vector<PathPoint> PathSegment::extract() {
  PointRefList temp = extractRef();
  return std::vector<PathPoint>(temp.begin(), temp.end());
}

PathSegment::PointRefList PathSegment::extractRef() {
  PointRefList temp;
  for (auto&& segment : segments) {
    if (std::holds_alternative<PathPoint>(segment)) {
      temp.push_back(std::get<PathPoint>(segment));
    } else {
      auto&& points = std::get<PathSegment>(segment).extractRef();
      for (auto&& point : points) {
        temp.push_back(point);
      }
    }
  }

  return temp;
}

} // namespace lib7842