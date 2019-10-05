#pragma once
#include "main.h"
#include "point.hpp"
#include <variant>
#include <list>

namespace lib7842 {

class PathSegment {

 public:
  PathSegment() = default;
  PathSegment(const PathSegment& isegment) = default;

  PathSegment& addPoint(const QPoint& ipoint);
  PathSegment& addSegment(const PathSegment& isegment);

  PathSegment& addPoints(const std::vector<QPoint>& ipoints);
  PathSegment& addSegments(const std::vector<PathSegment>& isegments);

  std::vector<QPoint> extract() const;

  using PointRefList = std::vector<std::reference_wrapper<QPoint>>;
  PointRefList extractRef();

 protected:
  std::list<std::variant<QPoint, PathSegment>> segments {};
};

} // namespace lib7842