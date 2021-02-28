#include "lib7842/api/trajectory/generator/skidGenerator.hpp"

namespace lib7842 {

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

} // namespace lib7842