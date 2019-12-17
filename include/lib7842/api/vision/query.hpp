#pragma once
#include <functional>

namespace lib7842::Vision {

class Object;

class Query : public std::function<double(const Object& obj)> {
public:
  using function::function;
  static Query sig, x, y, width, height, area, avgDim, centerX, centerY;

  double getFrom(const Object& obj) const;
};

} // namespace lib7842::Vision
