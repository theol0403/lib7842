#pragma once
#include "main.h"
#include "point.hpp"
#include <variant>

namespace lib7842 {

class PathPoint {

  public:
  PathPoint();
  PathPoint(QPoint ipoint);

  QLength x();
  QLength y();
  QPoint operator()();

  // void setDistance(QLength idistance);

  // void setVelocity(QSpeed ivelocity);
  // void setCurvature(double icurvature);

  protected:
  QPoint point {};
  std::map<std::string, std::variant<double, QLength, QSpeed>> pathData {};
};

} // namespace lib7842