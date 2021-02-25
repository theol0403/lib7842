#include "lib7842/api/trajectory/generator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

Generator::Generator(const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
  limits(ilimits), scales(iscales), dt(idt) {}

PiecewiseTrapezoidal
  Generator::generate(const Spline& spline, const Runner& runner, const ProfileFlags& flags,
                      const std::vector<std::pair<Number, Number>>& markers) const {
  QLength length = spline.length();
  PiecewiseTrapezoidal profile(limits, length, flags, markers);

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
  if (end.v == 0_mps) { runner({pos, end, 0_rpm, spline.curvature(1), end.v}); }
  return profile;
}

XGenerator::XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
                       const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
  Generator(ilimits, iscales, idt), model(std::move(imodel)), gearset(igearset) {
  {}
}

void XGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
                        const std::vector<std::pair<Number, Number>>& markers) {
  generate(
    spline,
    [&, rate = std::shared_ptr<AbstractRate>(global::getTimeUtil()->getRate())](const Step& s) {
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
    flags, markers);
}

std::tuple<std::vector<Generator::Step>, PiecewiseTrapezoidal>
  XTestGenerator::follow(const Spline& spline, bool /*forward*/, const ProfileFlags& flags,
                         const std::vector<std::pair<Number, Number>>& markers) {
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
    flags, markers);
  return {trajectory, profile};
}

} // namespace lib7842