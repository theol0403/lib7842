#pragma once
#include "pros/vision.h"
#include <functional>

namespace lib7842::Vision {

enum class Attribute {
  sig,
  x,
  y,
  width,
  height,

  area,
  avgDim,
  centerX,
  centerY,
  fromMidX,
  fromMidY,
  absFromMidX,
  absFromMidY
};

class Object {
public:
  Object() = default;
  Object(pros::vision_object);

protected:
  uint16_t _sig {0};
  double _x {0};
  double _y {0};
  double _width {0};
  double _height {0};

  void set(const Attribute& iattribute, double ivalue);
  double get(const Attribute& iattribute) const;
};

} // namespace lib7842::Vision
