#pragma once
#include "lib7842/api/other/units.hpp"
#include "okapi/api/chassis/controller/chassisScales.hpp"
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

template <class Unit = QLength> struct Limits {
  using Speed = decltype(Unit {1.0} / QTime {1.0});
  using Accel = decltype(Speed {1.0} / QTime {1.0});

  Accel a; // max acceleration
  Speed v; // max linear velocity

  Limits(const QAcceleration& ia, const Speed& iv) : a(ia), v(iv) {}

  Limits(const QTime& ia, const Speed& iv) : a(iv / ia), v(iv) {}
};

template <> struct Limits<> {
  QAcceleration a; // max acceleration
  QSpeed v; // max linear velocity
  QAngularSpeed w; // max angular velocity

  Limits(const QAcceleration& ia, const QSpeed& iv, const QAngularSpeed& iw) :
    a(ia), v(iv), w(iw) {}

  Limits(const QTime& ia, const QSpeed& iv, const QAngularSpeed& iw) : a(iv / ia), v(iv), w(iw) {}

  Limits(const QLength& idiam, const QAngularSpeed& igearset, const QLength& itrack,
         const QTime& ia, double iv = 1, double iw = 1) :
    Limits(ia, iv * idiam * pi * igearset / 360_deg, iw * idiam * igearset / itrack) {}

  Limits(const ChassisScales& iscales, const QAngularSpeed& igearset, const QTime& ia,
         double iv = 1, double iw = 1) :
    Limits(iscales.wheelDiameter, igearset, iscales.wheelTrack, ia, iv, iw) {}

  constexpr QSpeed max_vel_at_curvature(const QCurvature& c) const {
    return ((w * v) / (c.abs() * v * radian + w));
  }

  constexpr QSpeed max_vel_at_w(const QAngularSpeed& iw) const {
    return std::max(0_mps, v - (v * iw.abs()) / w);
  }
};

} // namespace lib7842
