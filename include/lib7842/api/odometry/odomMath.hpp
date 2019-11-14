#pragma once

#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"

namespace lib7842 {

namespace OdomMath {

Vector closest(const Vector& current, const QAngle& heading, const Vector& target);
Vector closest(const State& state, const Vector& target);

QAngle rollAngle360(const QAngle& angle);
QAngle rollAngle180(const QAngle& angle);
QAngle rollAngle90(const QAngle& angle);

} // namespace OdomMath

} // namespace lib7842
