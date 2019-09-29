#pragma once
#include "main.h"
#include "pathPoint.hpp"

namespace lib7842 {

class PathSegment {

 public:
  PathSegment() = default;
  PathSegment(const PathSegment& isegment) = default;

  PathSegment& addPoint(const PathPoint& ipoint);
  PathSegment& addSegment(const PathSegment& isegment);

  PathSegment& addPoints(const std::vector<PathPoint>& ipoints);
  PathSegment& addSegments(const std::vector<PathSegment>& isegments);

  std::vector<PathPoint> extract() const;

 protected:
  std::vector<std::variant<PathPoint, PathSegment>> segments {};
};

} // namespace lib7842