#pragma once
#include "lib7842/api/other/units.hpp"
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

struct Limits {
  QSpeed v; // max linear velocity
  QAngularSpeed w; // max angular velocity
  QAcceleration a; // max acceleration

  constexpr QSpeed max_vel_at_curvature(const QCurvature& c) const {
    return ((w * v) / (c.abs() * v * radian + w));
  }

  constexpr QSpeed max_vel_at_w(const QAngularSpeed& iw) const {
    return std::max(0_mps, v - (v * iw.abs()) / w);
  }
};

} // namespace lib7842