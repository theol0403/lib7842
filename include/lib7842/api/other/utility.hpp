#pragma once

#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/units/RQuantity.hpp"

namespace lib7842 {

using namespace okapi;

/////////////////
//    Units    //
/////////////////

/**
 * Unitless unit
 */
constexpr Number number(1.0);

///////////////////////
// Units - Curvature //
///////////////////////

/**
 * Curvature dimension which is length-1
 */
QUANTITY_TYPE(0, -1, 0, 0, QCurvature)

/**
 * Curvature unit
 */
constexpr QCurvature curvature(1.0);

/**
 * Curvature literals
 */
constexpr QCurvature operator"" _curv(long double x) {
  return QCurvature(x);
}
constexpr QCurvature operator"" _curv(unsigned long long int x) {
  return QCurvature(static_cast<double>(x));
}

//////////////////////
// Units - Velocity //
//////////////////////

/**
 * Inches per second
 */
constexpr QSpeed ips = inch / second;
constexpr QSpeed operator"" _ips(long double x) {
  return static_cast<double>(x) * ips;
}
constexpr QSpeed operator"" _ips(unsigned long long int x) {
  return static_cast<double>(x) * ips;
}

/**
 * Inches per second squared
 */
constexpr QAcceleration ips2 = inch / (second * second);
constexpr QAcceleration operator"" _ips2(long double x) {
  return static_cast<double>(x) * ips2;
}
constexpr QAcceleration operator"" _ips2(unsigned long long int x) {
  return static_cast<double>(x) * ips2;
}

///////////////////
// Units - Field //
///////////////////

/**
 * Units that represent a tile (2ft) and a court(12ft)
 * Literals are `_tl` and `_crt`, respectivly
 */
constexpr QLength tile = 2 * foot;
constexpr QLength court = 12 * foot;
constexpr QLength operator"" _tl(long double x) {
  return static_cast<double>(x) * tile;
}
constexpr QLength operator"" _crt(long double x) {
  return static_cast<double>(x) * court;
}
constexpr QLength operator"" _tl(unsigned long long int x) {
  return static_cast<double>(x) * tile;
}
constexpr QLength operator"" _crt(unsigned long long int x) {
  return static_cast<double>(x) * court;
}

} // namespace lib7842