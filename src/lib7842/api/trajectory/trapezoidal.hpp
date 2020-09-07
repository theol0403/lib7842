#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "profile.hpp"

namespace lib7842 {

struct TrapezoidalLimits {
  QSpeed v;
  QAngularSpeed w;
  QAcceleration a;
};

class Trapezoidal : public Profile {
public:
  constexpr Trapezoidal(const TrapezoidalLimits& ilimits, const QLength& ilength) :
    limits(ilimits), length(ilength) {
    // load limits
    auto& a = limits.a;
    auto& v = limits.v;

    // the time it takes to accelerate to full speed
    t_accel = v / a;
    // the time spent cruising at full speed
    t_cruise = (length - t_accel * v) / v;

    // if cruise time is negative, time needs to be shaved off the acceleration
    if (t_cruise < 0_s) {
      // this is a triangular profile
      t_cruise = 0_s;
      // maximum attainable speed given time constraints (if triangular)
      vel = sqrt(length * a);
      // time to accelerate to max speed
      t_accel = vel / a;
    } else {
      // this is not a triangular profile
      vel = v;
    }

    // the time it takes to complete the profile
    time = t_accel * 2 + t_cruise;

    // the distance to accelerate to full speed
    d_accel = 0.5 * a * (t_accel * t_accel);
    // the distance to cruise
    d_cruise = vel * t_cruise;
  }

  constexpr Kinematics calc(const QTime& t) const override {
    Kinematics k;
    if (t <= t_accel) {
      // acceleration
      k.a = limits.a;
      k.v = limits.a * t;
      k.d = 0.5 * limits.a * t * t;
    } else if (t > t_accel and t < t_accel + t_cruise) {
      // cruising
      k.a = 0_mps2;
      k.v = vel;
      k.d = d_accel + vel * (t - t_accel);
    } else {
      // deceleration
      k.a = limits.a * -1;
      QTime t_from_decel = (t - t_accel - t_cruise);
      k.v = vel - t_from_decel * limits.a;
      k.d = d_accel + d_cruise + vel * t_from_decel - 0.5 * limits.a * t_from_decel * t_from_decel;
    }
    k.t = t;
    return k;
  }

  constexpr Kinematics calc(const QLength& d) const override {
    QSpeed v = 0_mps;
    if (d <= d_accel) {
      // acceleration
      v = sqrt(2 * limits.a * d);
    } else if (d > d_accel and d < length - d_accel) {
      // cruising
      v = vel;
    } else {
      // deceleration
      QLength d_from_decel = d - d_accel - d_cruise;
      auto v_2 = vel * vel - 2 * limits.a * d_from_decel;
      if (v_2 < 0 * mps * mps) {
        v = 0_mps;
      } else {
        v = sqrt(v_2);
      }
    }
    Kinematics k;
    k.v = v;
    return k;
  }

protected:
  TrapezoidalLimits limits; // the kinematic limits

  QTime time; // the time it takes to complete the profile
  QLength length; // the length of the profile

  QSpeed vel; // the top speed reached during the profile

  QTime t_accel; // the time it takes to accelerate to full speed
  QTime t_cruise; // the time spent cruising at full speed

  QLength d_accel; // distance it takes to accelerate to full speed
  QLength d_cruise; // the distance spent cruising at full speed
};
} // namespace lib7842
