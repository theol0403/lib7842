#pragma once
#include "main.h"
#include "pathPoint.hpp"

namespace lib7842 {

class PathSegment {

 public:
  PathSegment() = default;
  PathSegment(const PathSegment& isegment) = default;

  void addPoint(const PathPoint& ipoint);
  void addSegment(const PathSegment& isegment);

  void addPoints(const std::vector<PathPoint>& ipoints);
  void addSegments(const std::vector<PathSegment>& isegments);

  std::vector<PathPoint> extract() const;

 protected:
  std::vector<std::variant<PathPoint, PathSegment>> segments {};
};

} // namespace lib7842