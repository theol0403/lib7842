#pragma once
#include <functional>

namespace lib7842::Vision {

struct Object;

class Query : public std::function<double(const Object& obj)> {
public:
  using function::function;
  static Query sig, x, y, width, height, fovWidth, fovHeight, area, avgDim, centerX, centerY;

  double getFrom(const Object& obj) const;
};

} // namespace lib7842::Vision
