#pragma once
#include "limits.hpp"
#include "profile.hpp"

namespace lib7842 {

template <class Unit = QLength> class Trapezoidal : public Profile<Unit> {
public:
  constexpr Trapezoidal(const Limits<Unit>& ilimits, const Unit& ilength,
                        const typename Profile<Unit>::Flags& iflags = {}) :
    limits(ilimits),
    length(ilength),
    start_v(limits.v * iflags.start_v),
    end_v(limits.v * iflags.end_v) {

    // load limits
    auto& a = limits.a;
    auto v = limits.v * iflags.top_v;

    auto offset = (square(start_v) + square(end_v)) / 2.0;

    // the distance spent cruising at full speed
    cruise_d = length + (offset - square(v)) / a;

    // if cruise distance is negative, time needs to be shaved off the acceleration
    if (cruise_d < Unit {0.0}) {
      // this is a triangular profile
      cruise_d = Unit {0.0};
      // maximum attainable speed given time constraints (if triangular)
      vel = sqrt(a * length + offset);
    } else {
      // this is not a triangular profile
      vel = v;
    }

    // time to accelerate to max speed
    accel_t = (vel - start_v) / a;
    decel_t = (vel - end_v) / a;
    if (accel_t < 0_s || decel_t < 0_s) {
      throw std::runtime_error("Impossible deceleration constraints");
    }

    // the distance to accelerate to max speed
    accel_d = start_v * accel_t + 0.5 * a * square(accel_t);
    decel_d = end_v * decel_t + 0.5 * a * square(decel_t);

    // the time to cruise
    cruise_t = cruise_d / vel;

    // the time it takes to complete the profile
    Profile<Unit>::time = accel_t + decel_t + cruise_t;
  }

  constexpr typename Profile<Unit>::State calc(QTime t) const override {
    typename Profile<Unit>::State k;
    if (t > Profile<Unit>::time) { t = Profile<Unit>::time; }
    if (t <= accel_t) {
      // acceleration
      k.a = limits.a;
      k.v = start_v + limits.a * t;
      k.d = start_v * t + 0.5 * limits.a * square(t);
    } else if (t > accel_t && t < accel_t + cruise_t) {
      // cruising
      k.v = vel;
      k.d = accel_d + vel * (t - accel_t);
    } else {
      // deceleration
      k.a = limits.a * -1;
      QTime t_from_decel = (t - accel_t - cruise_t);
      k.v = vel - t_from_decel * limits.a;
      k.d = accel_d + cruise_d + end_v * t_from_decel + 0.5 * limits.a * square(t_from_decel);
    }
    k.t = t;
    k.length = length;
    k.time = Profile<Unit>::time;
    k.vel = vel;
    return k;
  }

  constexpr typename Profile<Unit>::State calc(Unit d) const override {
    typename Profile<Unit>::State k;
    if (d > length) { d = length; }
    if (d <= accel_d) {
      // acceleration
      k.a = limits.a;
      k.v = sqrt(square(start_v) + 2 * limits.a * d);
      auto desc = sqrt(start_v * start_v + 2 * limits.a * d);
      k.t = (desc - start_v) / limits.a;
    } else if (d > accel_d && d < accel_d + cruise_d) {
      // cruising
      k.v = vel;
      k.t = accel_t + (d - accel_d) / vel;
    } else {
      // deceleration
      k.a = limits.a * -1;
      auto d_from_decel = d - accel_d - cruise_d;
      auto v_2 = square(vel) - 2 * limits.a * d_from_decel;
      if (v_2 < decltype(v_2) {0.0}) {
        k.v = typename Profile<Unit>::Speed {0.0};
      } else {
        k.v = sqrt(v_2);
      }
      auto desc = sqrt(vel * vel - 2 * limits.a * d_from_decel);
      k.t = accel_t + cruise_t + (desc - vel) / limits.a * -1;
    }
    k.d = d;
    k.length = length;
    k.time = Profile<Unit>::time;
    k.vel = vel;
    return k;
  }

  constexpr typename Profile<Unit>::State begin() const override {
    return {0_s, Unit {0.0}, limits.a, start_v};
  }

  constexpr typename Profile<Unit>::State end() const override {
    return {Profile<Unit>::time, length, -1 * limits.a, end_v};
  }

protected:
  Limits<Unit> limits; // the kinematic limits
  Unit length; // the length of the profile
  typename Profile<Unit>::Speed start_v; // the starting velocity
  typename Profile<Unit>::Speed end_v; // the ending velocity

  typename Profile<Unit>::Speed vel; // the top speed reached during the profile

  QTime accel_t; // the time it takes to accelerate to full speed
  QTime decel_t; // the time it takes to decelerate
  QTime cruise_t; // the time spent cruising at full speed

  Unit accel_d; // distance it takes to accelerate to full speed
  Unit decel_d; // distance it takes to decelerate
  Unit cruise_d; // the distance spent cruising at full speed
};
} // namespace lib7842
