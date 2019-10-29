#pragma once
#include "main.h"
#include "lib7842/positioning/point/vector.hpp"
#include "lib7842/positioning/point/state.hpp"

namespace lib7842 {

namespace lib7842::OdomMath {

Vector closest(const Vector&, const Vector&, const Vector&);
Vector closest(const State&, const Vector&);

QAngle rollAngle360(const QAngle&);
QAngle rollAngle180(const QAngle&);
QAngle rollAngle90(const QAngle&);

} // namespace lib7842::OdomMath

} // namespace lib7842
