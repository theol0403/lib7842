#pragma once
#include "main.h"
#include "lib7842/positioning/point/vector.hpp"

namespace lib7842 {

namespace lib7842::OdomMath {

// dPoint closest(const dPoint&, const dPoint&, const dPoint&);
// Vector closest(const Vector&, const Vector&);

QAngle rollAngle360(const QAngle&);
QAngle rollAngle180(const QAngle&);
QAngle rollAngle90(const QAngle&);

} // namespace lib7842::OdomMath

} // namespace lib7842
