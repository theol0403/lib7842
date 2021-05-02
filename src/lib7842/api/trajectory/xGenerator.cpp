#include "lib7842/api/trajectory/generator/xGenerator.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Generator::Output XGenerator::follow(const Spline& spline, const XFlags& flags,
                                     const PiecewiseTrapezoidal::Markers& markers) {
  std::vector<Generator::Step> trajectory;

  if (model && flags.start_v == 0_pct) {
    model->stop();
    pros::delay(10);
  }

  // the robots heading
  QAngle robot = flags.start;

  auto runner = [&](double t, Profile<>::State& k) {
    auto profiled_vel = k.v; // used for logging
    auto w = flags.rotator(k);

    // get the location on the spline
    auto pos = spline.calc(t);
    auto curvature = spline.curvature(t);
    pos.theta = pos.theta - robot;

    // this is experimental
    auto scale = (sin(pos.theta).abs() + cos(pos.theta).abs());
    k.v = k.v / scale;
    if (flags.curvature) {
      k.v = std::min(k.v, (limits.w * limits.v / scale) /
                            (curvature.abs() * limits.v / scale * radian + limits.w));
    } else {
      k.v = std::min(k.v, limits.v / scale - w.abs() * limits.v / limits.w);
    }

    // angular speed is curvature times limited speed
    if (flags.curvature) { w += curvature * k.v * radian; }

    robot += w * dt;

    auto turning = -(w / radian * scales.wheelTrack) / 2;
    auto left = k.v * sin(pos.theta + 45_deg);
    auto right = k.v * sin(pos.theta - 45_deg);

    auto topLeft = left + turning;
    auto topRight = right - turning;
    auto bottomLeft = right + turning;
    auto bottomRight = left - turning;

    auto topLeftSpeed = Generator::toWheel(topLeft, scales, gearset).convert(number);
    auto topRightSpeed = Generator::toWheel(topRight, scales, gearset).convert(number);
    auto bottomLeftSpeed = Generator::toWheel(bottomLeft, scales, gearset).convert(number);
    auto bottomRightSpeed = Generator::toWheel(bottomRight, scales, gearset).convert(number);

    if (model) {
      model->getTopLeftMotor()->moveVelocity(topLeftSpeed * gearset.convert(rpm));
      model->getTopRightMotor()->moveVelocity(topRightSpeed * gearset.convert(rpm));
      model->getBottomLeftMotor()->moveVelocity(bottomLeftSpeed * gearset.convert(rpm));
      model->getBottomRightMotor()->moveVelocity(bottomRightSpeed * gearset.convert(rpm));
    }

    trajectory.emplace_back(pos, k, w, spline.curvature(t), profiled_vel, topLeftSpeed,
                            topRightSpeed, bottomLeftSpeed, bottomRightSpeed,
                            pos.theta - 90_deg - robot);
  };

  auto profile = Generator::generate(limits, runner, spline, dt, flags, markers);
  return std::make_pair(profile, trajectory);
}

} // namespace lib7842
