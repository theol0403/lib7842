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

std::vector<PathPoint> PathSegment::extract() const {
  return {};
}

} // namespace lib7842