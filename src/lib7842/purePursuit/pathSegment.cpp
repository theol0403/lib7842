#include "pathSegment.hpp"

namespace lib7842 {

PathSegment::PathSegment(const segment_t& isegment) {
  addSegment(isegment);
}

PathSegment::PathSegment(const segments_t& isegments) {
  addSegments(isegments);
}

void PathSegment::addSegment(const segment_t& isegment) {
  segments.push_back(isegment);
}

void PathSegment::addSegments(const segments_t& isegments) {
  for (auto&& isegment : isegments) {
    segments.push_back(isegment);
  }
}

PathPoint PathSegment::extractPoint(const segment_t& isegment) const {
  if (std::holds_alternative<PathPoint>(isegment)) {
    return std::get<PathPoint>(isegment);
  } else {
    return extractPoint(std::get<PathSegment>(isegment));
  }
}

std::vector<PathPoint> PathSegment::extract() const {
  std::vector<PathPoint> temp;
  for (auto&& segment : segments) {
    temp.push_back(extractPoint(segment));
  }
  return temp;
}

} // namespace lib7842