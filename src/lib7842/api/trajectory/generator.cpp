#include "lib7842/api/trajectory/generator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

PiecewiseTrapezoidal Generator::generate(const Limits& limits, const Modifier& modifier,
                                         const Executor& executor, const Spline& spline,
                                         const QTime& dt, const ProfileFlags& flags,
                                         const std::vector<std::pair<Number, Number>>& markers) {
  auto rate = global::getTimeUtil()->getRate();
  QLength length = spline.length();
  PiecewiseTrapezoidal profile(limits, length, flags, markers);

  // setup
  double t = 0;
  QLength dist = 0_m;
  KinematicState k = profile.begin();
  if (k.v == 0_mps) { k = profile.calc(dt); }

  while (dist <= length && t <= 1) {
    // calculate and run motion along trajectory
    executor(modifier(t, k));

    // calculate distance traveled
    QLength d_dist = k.v * dt;
    dist += d_dist;
    // calculate where along the spline we will be at the end of the timeslice
    t = spline.t_at_dist_travelled(t, d_dist);
    // calculate new velocity
    k = profile.calc(dist);

    rate->delayUntil(dt);
  }
  KinematicState end = profile.end();
  if (end.v == 0_mps) { executor(modifier(1, end)); }
  return profile;
}

Number Generator::toWheel(const QSpeed& v, const ChassisScales& scales,
                          const QAngularSpeed& gearset) {
  return (v / (1_pi * scales.wheelDiameter * gearset)) * 360_deg;
}

Generator::Output
  SkidSteerGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
                             const std::vector<std::pair<Number, Number>>& markers) {
  std::vector<Generator::Step> trajectory;
  auto modifier = [&](double t, KinematicState& k) {
    auto profiled_vel = k.v; // used for logging

    // get the curvature along the path
    auto curvature = spline.curvature(t);
    // limit the velocity according to curvature.
    // since this is passed by reference it will affect the generator code
    k.v = std::min(k.v, limits.max_vel_at_curvature(curvature));

    // angular speed is curvature times limited speed
    QAngularSpeed w = curvature * k.v * radian;

    // scale down motor speed if x drive
    auto vel = k.v;
    if (isXdrive) { vel /= std::sqrt(2); }

    QSpeed left = vel - (w / radian * scales.wheelTrack) / 2;
    QSpeed right = vel + (w / radian * scales.wheelTrack) / 2;

    Number leftSpeed = Generator::toWheel(left, scales, gearset);
    Number rightSpeed = Generator::toWheel(right, scales, gearset);

    trajectory.emplace_back(spline.calc(t), k, w, curvature, profiled_vel, leftSpeed, rightSpeed);

    if (!forward) { return std::make_pair(-rightSpeed, -leftSpeed); }
    return std::make_pair(leftSpeed, rightSpeed);
  };

  auto e = [&](const Generator::DriveCommand& c) { executor(c); };
  auto profile = Generator::generate(limits, modifier, e, spline, dt, flags, markers);
  return std::make_pair(profile, trajectory);
}

void SkidSteerGenerator::executor(const Generator::DriveCommand& c) {
  if (!model) { return; }
  double left = c.first.convert(number);
  double right = c.second.convert(number);
  model->tank(left, right);
}

Generator::Output XGenerator::follow(const Spline& spline, const ProfileFlags& flags,
                                     const std::vector<std::pair<Number, Number>>& markers) {
  std::vector<Generator::Step> trajectory;
  auto modifier = [&](double t, KinematicState& k) {
    auto profiled_vel = k.v; // used for logging

    // get the location on the spline
    auto pos = spline.calc(t);
    auto& theta = pos.theta;

    // limit the velocity according to path angle.
    // since this is passed by reference it will affect the generator code
    k.v = std::min(k.v, limits.v / (sin(theta).abs() + cos(theta).abs()));

    auto left = k.v * sin(theta + 45_deg);
    auto right = k.v * sin(theta - 45_deg);

    Number topLeftSpeed = Generator::toWheel(left, scales, gearset);
    Number topRightSpeed = Generator::toWheel(right, scales, gearset);

    trajectory.emplace_back(pos, k, 0_rpm, spline.curvature(t), profiled_vel, topLeftSpeed,
                            topRightSpeed);

    return std::make_pair(topLeftSpeed, topRightSpeed);
  };

  auto e = [&](const Generator::DriveCommand& c) { executor(c); };
  auto profile = Generator::generate(limits, modifier, e, spline, dt, flags, markers);
  return std::make_pair(profile, trajectory);
}

void XGenerator::executor(const Generator::DriveCommand& c) {
  if (!model) { return; }
  double topLeft = c.first.convert(number);
  double topRight = c.second.convert(number);

  model->getTopLeftMotor()->moveVoltage(topLeft * 12000);
  model->getTopRightMotor()->moveVoltage(topRight * 12000);
  model->getBottomLeftMotor()->moveVoltage(topRight * 12000);
  model->getBottomRightMotor()->moveVoltage(topLeft * 12000);
}

} // namespace lib7842