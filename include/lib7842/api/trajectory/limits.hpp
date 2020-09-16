#pragma once
#include "lib7842/api/other/units.hpp"
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

struct Limits {
  QAcceleration a; // max acceleration
  QSpeed v; // max linear velocity
  QAngularSpeed w; // max angular velocity

  constexpr QSpeed max_vel_at_curvature(const QCurvature& c) const {
    return ((w * v) / (c.abs() * v * radian + w));
  }

  constexpr QSpeed max_vel_at_w(const QAngularSpeed& iw) const {
    return std::max(0_mps, v - (v * iw.abs()) / w);
  }

  Limits(const QAcceleration& ia, const QSpeed& iv, const QAngularSpeed& iw) :
    a(ia), v(iv), w(iw) {}

  Limits(const QTime& ia, const QSpeed& iv, const QAngularSpeed& iw) : a(iv / ia), v(iv), w(iw) {}

  Limits(const QLength& idiam, const QAngularSpeed& igearset, const QLength& itrack,
         const QTime& ia, double iv = 1, double iw = 1) :
    Limits(ia, iv * idiam * igearset / radian, iw * 2 * idiam * igearset / itrack) {}
};

} // namespace lib7842