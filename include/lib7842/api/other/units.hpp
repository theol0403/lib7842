#pragma once
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/RQuantity.hpp"

namespace lib7842 {

using namespace okapi;
QUANTITY_TYPE(0, -1, 0, 0, QCurvature);

/**
 * Units that represent a percentage
 */
constexpr Number percent(0.01);
constexpr Number operator"" _pct(long double x) { return static_cast<double>(x) * percent; }
constexpr Number operator"" _pct(unsigned long long int x) {
  return static_cast<double>(x) * percent;
}

namespace units {

/**
 * Units that represent a tile (2ft) and a court(12ft)
 * Literals are `_tile` and `_court`, respectively
 */
constexpr QLength tile = 2 * foot;
constexpr QLength court = 12 * foot;
constexpr QLength operator"" _tile(long double x) { return static_cast<double>(x) * tile; }
constexpr QLength operator"" _court(long double x) { return static_cast<double>(x) * court; }
constexpr QLength operator"" _tile(unsigned long long int x) {
  return static_cast<double>(x) * tile;
}
constexpr QLength operator"" _court(unsigned long long int x) {
  return static_cast<double>(x) * court;
}

} // namespace units
} // namespace lib7842