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

class Generator {
public:
  struct Step {
    State p;
    KinematicState k;
    QAngularSpeed w;
    QCurvature c;
    QSpeed p_vel;
    Number left {0_pct};
    Number right {0_pct};
  };

  using Runner = std::function<void(const Step&)>;

  virtual ~Generator() = default;

  Generator(const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
    limits(ilimits), scales(iscales), dt(idt) {}

  Trapezoidal generate(const Spline& spline, const Runner& runner,
                       const ProfileFlags& iflags = {}) const {
    QLength length = spline.length();
    Trapezoidal profile(limits, length, iflags);

    // setup
    double t = 0;
    QLength dist = 0_m;
    State pos = spline.calc(t);
    KinematicState k = profile.begin();
    if (k.v == 0_mps) { k = profile.calc(dt); }

    while (dist <= length && t <= 1) {
      auto profiled_vel = k.v; // used for logging
      // limit velocity according to approximation of the curvature during the next timeslice
      QCurvature curvature = spline.curvature(t);
      QSpeed vel_max = std::min(k.v, limits.max_vel_at_curvature(curvature));
      // project where along the spline we will be after dt, given approximate velocity
      double t_n = spline.t_at_dist_travelled(t, vel_max * dt);
      State pos_new = spline.calc(t_n);

      // find out how fast we need to turn to achieve change in theta to reach next point in dt
      QAngularSpeed angular_vel = (pos_new.theta - pos.theta) / dt;
      // update internal theta representation
      // limit profiled velocity to angular velocity
      k.v = std::min(k.v, limits.max_vel_at_w(angular_vel));

      // calculate distance traveled
      QLength d_dist = k.v * dt;
      dist += d_dist;
      // calculate where along the spline we will be at the end of the timeslice
      t = spline.t_at_dist_travelled(t, d_dist);

      // run trajectory
      runner({pos, k, angular_vel, curvature, profiled_vel});

      // update new position
      pos = spline.calc(t);
      // calculate new velocity
      k = profile.calc(dist);
    }
    KinematicState end = profile.end();
    if (end.v == 0_mps) { runner({pos, end, 0_rpm, 0 / 1_m, end.v}); }
    return profile;
  }

protected:
  Limits limits;
  ChassisScales scales;
  QTime dt;
};

class SkidSteerGenerator : public Generator {
public:
  SkidSteerGenerator(std::shared_ptr<ChassisModel> imodel, const QAngularSpeed& igearset,
                     const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
    Generator(ilimits, iscales, idt), model(std::move(imodel)), gearset(igearset) {
    {}
  }

  void follow(const Spline& spline, bool forward = true, const ProfileFlags& flags = {}) {
    generate(
      spline,
      [&, rate = std::make_shared<Rate>()](const Step& s) {
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
      flags);
  }

protected:
  std::shared_ptr<ChassisModel> model;
  QAngularSpeed gearset;
};

class XGenerator : public Generator {
public:
  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
    Generator(ilimits, iscales, idt), model(std::move(imodel)), gearset(igearset) {
    {}
  }

  void follow(const Spline& spline, bool forward = true, const ProfileFlags& flags = {}) {
    generate(
      spline,
      [&, rate = std::make_shared<Rate>()](const Step& s) {
        QSpeed left = s.k.v / std::sqrt(2) - (s.w / radian * scales.wheelTrack) / 2;
        QSpeed right = s.k.v / std::sqrt(2) + (s.w / radian * scales.wheelTrack) / 2;

        QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
        QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

        auto leftSpeed = (leftWheel / gearset).convert(number);
        auto rightSpeed = (rightWheel / gearset).convert(number);

        if (forward) {
          model->tank(leftSpeed, rightSpeed);
        } else {
          model->tank(-rightSpeed, -leftSpeed);
        }
        rate->delayUntil(dt);
      },
      flags);
  }

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
};

class XTestGenerator : public Generator {
public:
  using Generator::Generator;
  std::tuple<std::vector<Step>, Trapezoidal> follow(const Spline& spline, bool /*forward*/ = true,
                                                    const ProfileFlags& flags = {}) {
    std::vector<Step> trajectory;
    auto profile = generate(
      spline,
      [&](const Step& s) {
        QSpeed left = s.k.v / sqrt(2) - (s.w / radian * scales.wheelTrack) / 2;
        QSpeed right = s.k.v / sqrt(2) + (s.w / radian * scales.wheelTrack) / 2;

        QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
        QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

        auto leftSpeed = leftWheel / 200_rpm;
        auto rightSpeed = rightWheel / 200_rpm;

        Step ns = s;
        ns.left = leftSpeed;
        ns.right = rightSpeed;
        trajectory.emplace_back(ns);
      },
      flags);
    return {trajectory, profile};
  }
};

class TestGenerator : public Generator {
public:
  using Generator::Generator;
  std::vector<Step> follow(const Spline& spline, const ProfileFlags& flags = {}) {
    std::vector<Step> trajectory;
    generate(
      spline, [&](const Step& s) { trajectory.emplace_back(s); }, flags);
    return trajectory;
  }
};

} // namespace lib7842