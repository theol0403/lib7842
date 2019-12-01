#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/units/RQuantity.hpp"

namespace lib7842::units {

using namespace okapi;

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

/**
 * Units that represent a tile (2ft) and a court(12ft)
 * Literals are `_tile` and `_court`, respectivly
 */
constexpr QLength tile = 2 * foot;
constexpr QLength court = 12 * foot;
constexpr QLength operator"" _tile(long double x) {
  return static_cast<double>(x) * tile;
}
constexpr QLength operator"" _court(long double x) {
  return static_cast<double>(x) * court;
}
constexpr QLength operator"" _tile(unsigned long long int x) {
  return static_cast<double>(x) * tile;
}
constexpr QLength operator"" _court(unsigned long long int x) {
  return static_cast<double>(x) * court;
}

} // namespace lib7842::units