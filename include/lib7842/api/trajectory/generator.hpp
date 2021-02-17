#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/spline/spline.hpp"
#include "limits.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/impl/util/rate.hpp"
#include "trapezoidal.hpp"

namespace lib7842 {

class TrajectoryGenerator {
public:
  struct Step {
    State p;
    QSpeed v;
    QAngularSpeed w;
    QCurvature c;
  };

  static std::vector<Step> generate(const Spline& spline, const Limits& limits,
                                    const QSpeed& start_v = 0_mps, const QSpeed& end_v = 0_mps,
                                    const QTime& dt = 10_ms) {
    QLength length = spline.length();
    Trapezoidal profile(limits, length, start_v, end_v);

    std::vector<Step> trajectory;

    // setup
    double t = 0;
    QLength dist = 0_m;
    State pos = spline.calc(t);
    QAngle theta = pos.theta;
    QSpeed vel = profile.calc(0_s).v;
    if (vel == 0_mps) { vel = profile.calc(dt).v; }

    while (dist <= length && t <= 1) {
      // limit velocity according to approximation of the curvature during the next timeslice
      QCurvature curvature = spline.curvature(t);
      QSpeed vel_max = std::min(vel, limits.max_vel_at_curvature(curvature));
      // project where along the spline we will be after dt, given approximate velocity
      double t_n = spline.t_at_dist_travelled(t, vel_max * dt);
      State pos_new = spline.calc(t_n);

      // find out how fast we need to turn to achieve change in theta to reach next point in dt
      QAngularSpeed angular_vel = (pos_new.theta - theta) / dt;
      // update internal theta representation
      theta += angular_vel * dt;
      // limit profiled velocity to angular velocity
      vel = std::min(vel, limits.max_vel_at_w(angular_vel));

      // calculate distance traveled
      QLength d_dist = vel * dt;
      dist += d_dist;
      // calculate where along the spline we will be at the end of the timeslice
      t = spline.t_at_dist_travelled(t, d_dist);

      // save trajectory
      trajectory.emplace_back(pos, vel, angular_vel, curvature);

      // update new position
      pos = spline.calc(t);
      // calculate new velocity
      vel = profile.calc(dist).v;
    }
    return trajectory;
  }

  static void follow(ChassisModel& chassis, const std::vector<Step>& trajectory,
                     const ChassisScales& scales, const QAngularSpeed& igearset,
                     bool forward = true) {
    Rate rate;
    for (const auto& step : trajectory) {
      QSpeed left = step.v - (step.w / radian * scales.wheelTrack) / 2;
      QSpeed right = step.v + (step.w / radian * scales.wheelTrack) / 2;

      QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
      QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

      auto leftSpeed = (leftWheel / igearset).convert(number);
      auto rightSpeed = (rightWheel / igearset).convert(number);

      if (forward) {
        chassis.tank(leftSpeed, rightSpeed);
      } else {
        chassis.tank(-rightSpeed, -leftSpeed);
      }
      // chassis.left(leftSpeed);
      // chassis.right(rightSpeed);
      rate.delayUntil(10_ms);
    }
    chassis.forward(0);
  }
};

} // namespace lib7842