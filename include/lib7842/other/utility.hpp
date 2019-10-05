#pragma once
#include "main.h"

namespace lib7842 {

/**
 * Define unitless unit
 */
constexpr Number number(1.0);

/**
 * Define Curvature dimention which is length-1
 */
QUANTITY_TYPE(0, -1, 0, 0, QCurvature)

/**
 * Curvature unit and literals
 */
constexpr QCurvature curvature(1.0);
constexpr QCurvature operator"" _curv(long double x) {
  return QCurvature(x);
}
constexpr QCurvature operator"" _curv(unsigned long long int x) {
  return QCurvature(static_cast<double>(x));
}

} // namespace lib7842