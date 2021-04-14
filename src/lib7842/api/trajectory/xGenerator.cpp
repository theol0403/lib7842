#include "lib7842/api/trajectory/generator/xGenerator.hpp"

namespace lib7842 {

Generator::Output XGenerator::follow(const Spline& spline, const ProfileFlags& flags,
                                     const PiecewiseTrapezoidal::Markers& markers) {
  std::vector<Generator::Step> trajectory;
  auto runner = [&](double t, KinematicState& k) {
    auto profiled_vel = k.v; // used for logging

    // get the location on the spline
    auto pos = spline.calc(t);
    auto& theta = pos.theta;

    // limit the velocity according to path angle.
    // since this is passed by reference it will affect the generator code
    // k.v = k.v / (sin(theta).abs() + cos(theta).abs());
    k.v = std::min(k.v, limits.v / (sin(theta).abs() + cos(theta).abs()));

    auto left = k.v * sin(theta + 45_deg);
    auto right = k.v * sin(theta - 45_deg);

    Number topLeftSpeed = Generator::toWheel(left, scales, gearset);
    Number topRightSpeed = Generator::toWheel(right, scales, gearset);

    if (model) {
      double topLeft = topLeftSpeed.convert(number);
      double topRight = topRightSpeed.convert(number);

      model->getTopLeftMotor()->moveVelocity(topLeft * gearset.convert(rpm));
      model->getTopRightMotor()->moveVelocity(topRight * gearset.convert(rpm));
      model->getBottomLeftMotor()->moveVelocity(topRight * gearset.convert(rpm));
      model->getBottomRightMotor()->moveVelocity(topLeft * gearset.convert(rpm));
    }

    trajectory.emplace_back(pos, k, 0_rpm, spline.curvature(t), profiled_vel, topLeftSpeed,
                            topRightSpeed);
  };

  auto profile = Generator::generate(limits, runner, spline, dt, flags, markers);
  return std::make_pair(profile, trajectory);
}

} // namespace lib7842