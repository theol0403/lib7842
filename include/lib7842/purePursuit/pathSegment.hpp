#pragma once
#include "main.h"
#include "pathPoint.hpp"

namespace lib7842 {

class PathSegment {

  // using segment_t = std::variant<PathPoint, PathSegment>;
  using segment_t = PathPoint;
  using segments_t = std::vector<segment_t>;

 public:
  PathSegment(const segment_t& isegment);
  PathSegment(const segments_t& isegments);

  void addSegment(const segment_t& isegment);
  void addSegments(const segment_t& isegments);

  segments_t concat() const;

 protected:
  segments_t segments {};
};

} // namespace lib7842