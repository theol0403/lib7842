#pragma once
#include "limits.hpp"
#include "profile.hpp"

namespace lib7842 {

class Trapezoidal : public Profile {
public:
  constexpr Trapezoidal(const Limits& ilimits, const QLength& ilength) :
    limits(ilimits), length(ilength) {
    // load limits
    auto& a = limits.a;
    auto& v = limits.v;

    // the time it takes to accelerate to full speed
    accel_t = v / a;
    // the time spent cruising at full speed
    cruise_t = (length - accel_t * v) / v;

    // if cruise time is negative, time needs to be shaved off the acceleration
    if (cruise_t < 0_s) {
      // this is a triangular profile
      cruise_t = 0_s;
      // maximum attainable speed given time constraints (if triangular)
      vel = sqrt(length * a);
      // time to accelerate to max speed
      accel_t = vel / a;
    } else {
      // this is not a triangular profile
      vel = v;
    }

    // the time it takes to complete the profile
    time = accel_t * 2 + cruise_t;

    // the distance to accelerate to full speed
    accel_d = 0.5 * a * (accel_t * accel_t);
    // the distance to cruise
    cruise_d = vel * cruise_t;
  }

  constexpr KinematicState calc(const QTime& t) const override {
    KinematicState k;
    if (t <= accel_t) {
      // acceleration
      k.a = limits.a;
      k.v = limits.a * t;
    } else if (t > accel_t and t < accel_t + cruise_t) {
      // cruising
      k.v = vel;
    } else {
      // deceleration
      k.a = limits.a * -1;
      QTime t_from_decel = (t - accel_t - cruise_t);
      k.v = vel - t_from_decel * limits.a;
    }
    k.t = t;
    return k;
  }

  constexpr KinematicState calc(const QLength& d) const override {
    KinematicState k;
    if (d <= accel_d) {
      // acceleration
      k.a = limits.a;
      k.v = sqrt(2 * limits.a * d);
    } else if (d > accel_d and d < length - accel_d) {
      // cruising
      k.a = 0_mps2;
      k.v = vel;
    } else {
      // deceleration
      k.a = limits.a * -1;
      QLength d_from_decel = d - accel_d - cruise_d;
      auto v_2 = vel * vel - 2 * limits.a * d_from_decel;
      if (v_2 < 0 * mps * mps) {
        k.v = 0_mps;
      } else {
        k.v = sqrt(v_2);
      }
    }
    k.d = d;
    return k;
  }

protected:
  Limits limits; // the kinematic limits

  QTime time; // the time it takes to complete the profile
  QLength length; // the length of the profile

  QSpeed vel; // the top speed reached during the profile

  QTime accel_t; // the time it takes to accelerate to full speed
  QTime cruise_t; // the time spent cruising at full speed

  QLength accel_d; // distance it takes to accelerate to full speed
  QLength cruise_d; // the distance spent cruising at full speed
};
} // namespace lib7842
