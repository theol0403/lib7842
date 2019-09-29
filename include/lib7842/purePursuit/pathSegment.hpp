#pragma once
#include "main.h"
#include "pathPoint.hpp"

namespace lib7842 {

class PathSegment {

  using segment_t = std::variant<PathPoint, PathSegment>;

 public:
  PathSegment() = default;

  PathSegment(const PathPoint& ipoint);
  PathSegment(const PathSegment& isegment) = default;
  PathSegment(const segment_t& isegment);

  PathSegment(const std::vector<PathPoint>& ipoints);
  PathSegment(const std::vector<PathSegment>& isegments);
  PathSegment(const std::vector<segment_t>& isegments);

  void addPoint(const PathPoint& ipoint);
  void addSegment(const PathSegment& isegment);
  void addSegment(const segment_t& isegment);

  void addPoints(const std::vector<PathPoint>& ipoints);
  void addSegments(const std::vector<PathSegment>& isegments);
  void addSegments(const std::vector<segment_t>& isegments);

  std::vector<PathPoint> extract() const;

 protected:
  std::vector<segment_t> segments {};
};

} // namespace lib7842