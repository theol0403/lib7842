#pragma once
#include "pros/vision.h"
#include "query.hpp"
#include <functional>

namespace lib7842::Vision {

class Object {
public:
  Object() = default;
  explicit Object(const pros::vision_object& iobject);

  friend class Query;

protected:
  uint16_t sig {0};
  double x {0};
  double y {0};
  double width {0};
  double height {0};

  void set(const Query& iquery, double ivalue);
  double get(const Query& iquery) const;
};

} // namespace lib7842::Vision
