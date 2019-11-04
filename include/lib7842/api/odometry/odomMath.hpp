#pragma once
#include "main.h"
#include "lib7842/api/positioning/point/vector.hpp"
#include "lib7842/api/positioning/point/state.hpp"

namespace lib7842 {

namespace lib7842::OdomMath {

Vector closest(const Vector& current, const QAngle& heading, const Vector& target);
Vector closest(const State& state, const Vector& target);

QAngle rollAngle360(const QAngle& angle);
QAngle rollAngle180(const QAngle& angle);
QAngle rollAngle90(const QAngle& angle);

} // namespace lib7842::OdomMath

} // namespace lib7842
