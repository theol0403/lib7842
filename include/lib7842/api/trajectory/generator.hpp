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
  TrajectoryGenerator(std::shared_ptr<ChassisModel> imodel, const Limits& ilimits,
                      const ChassisScales& iscales, const QAngularSpeed& igearset,
                      const QTime& idt) :
    model(std::move(imodel)), limits(ilimits), scales(iscales), gearset(igearset), dt(idt) {
    {}
  }

  struct Step {
    State p;
    KinematicState k;
    QAngularSpeed w;
  };

  using Runner = std::function<void(const Step&)>;

  void generate(const Spline& spline, const Runner& runner, const Number& istart_v = 0_pct,
                const Number& iend_v = 0_pct, const Number& itop_v = 100_pct) const {
    QLength length = spline.length();
    Trapezoidal profile(limits, length, istart_v, iend_v, itop_v);

    // setup
    double t = 0;
    QLength dist = 0_m;
    State pos = spline.calc(t);
    QAngle theta = pos.theta;
    KinematicState k = profile.begin();
    if (k.v == 0_mps) { k = profile.calc(dt); }

    while (dist <= length && t <= 1) {
      // limit velocity according to approximation of the curvature during the next timeslice
      QCurvature curvature = spline.curvature(t);
      QSpeed vel_max = std::min(k.v, limits.max_vel_at_curvature(curvature));
      // project where along the spline we will be after dt, given approximate velocity
      double t_n = spline.t_at_dist_travelled(t, vel_max * dt);
      State pos_new = spline.calc(t_n);

      // find out how fast we need to turn to achieve change in theta to reach next point in dt
      QAngularSpeed angular_vel = (pos_new.theta - theta) / dt;
      // update internal theta representation
      theta += angular_vel * dt;
      // limit profiled velocity to angular velocity
      k.v = std::min(k.v, limits.max_vel_at_w(angular_vel));

      // calculate distance traveled
      QLength d_dist = k.v * dt;
      dist += d_dist;
      // calculate where along the spline we will be at the end of the timeslice
      t = spline.t_at_dist_travelled(t, d_dist);

      // run trajectory
      runner({pos, k, angular_vel});

      // update new position
      pos = spline.calc(t);
      // calculate new velocity
      k = profile.calc(dist);
    }
    KinematicState end = profile.end();
    if (end.v == 0_mps) { runner({pos, end, 0_rpm}); }
  }

  void follow(const Spline& spline, bool forward = true, const Number& istart_v = 0_pct,
              const Number& iend_v = 0_pct, const Number& itop_v = 100_pct) {
    generate(
      spline,
      [&, rate = std::shared_ptr<Rate>()](const Step& s) {
        QSpeed left = s.k.v - (s.w / radian * scales.wheelTrack) / 2;
        QSpeed right = s.k.v + (s.w / radian * scales.wheelTrack) / 2;

        QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
        QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

        auto leftSpeed = (leftWheel / gearset).convert(number);
        auto rightSpeed = (rightWheel / gearset).convert(number);

        if (forward) {
          model->tank(leftSpeed, rightSpeed);
        } else {
          model->tank(-rightSpeed, -leftSpeed);
        }
        // model.left(leftSpeed);
        // model.right(rightSpeed);

        rate->delayUntil(dt);
      },
      istart_v, iend_v, itop_v);
  }

protected:
  std::shared_ptr<ChassisModel> model;
  Limits limits;
  ChassisScales scales;
  QAngularSpeed gearset;
  QTime dt;
};

} // namespace lib7842