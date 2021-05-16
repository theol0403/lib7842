#include "lib7842/api/trajectory/generator/xGenerator.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Generator::Output XGenerator::follow(const Spline& spline, const XFlags& flags,
                                     const PiecewiseTrapezoidal::Markers& markers) {
#ifdef THREADS_STD
  std::vector<Generator::Step> trajectory;
#endif

  if (model && flags.start_v == 0_pct) {
    model->stop();
    pros::delay(10);
  }

  // the robots heading
  QAngle robot = flags.start.value_or(spline.calc(0).theta);

  auto runner = [&](double t, Profile<>::State& k) {
#ifdef THREADS_STD
    auto profiled_vel = k.v; // used for logging
#endif
    auto angler = flags.steerer(k);
    auto w = flags.rotator(k) + angler;
    w = std::clamp(w, -limits.w, limits.w);

    // get the location on the spline
    auto pos = spline.calc(t);
    auto curvature = spline.curvature(t);
    pos.theta = pos.theta - robot + flags.strafer(k);

    // this is experimental
    auto scale = (sin(pos.theta).abs() + cos(pos.theta).abs());
    k.v = k.v / scale;
    if (flags.curve) {
      k.v = std::min(k.v, (limits.w * limits.v / scale) /
                            (curvature.abs() * limits.v / scale * radian + limits.w));
    } else {
      k.v = std::min(k.v, limits.v / scale - w.abs() * limits.v / limits.w);
    }

    // angular speed is curvature times limited speed
    if (flags.curve) { w += curvature * k.v * radian; }

    robot += (w - angler) * dt;

    auto turning = -(w / radian * scales.wheelTrack) / 2;
    auto left = k.v * cos(pos.theta + 45_deg);
    auto right = k.v * cos(pos.theta - 45_deg);

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

#ifdef THREADS_STD
    trajectory.emplace_back(pos, k, w, spline.curvature(t), profiled_vel, topLeftSpeed,
                            topRightSpeed, bottomLeftSpeed, bottomRightSpeed);
#endif
  };

  auto profile = Generator::generate(limits, runner, spline, dt,
                                     {flags.start_v, flags.end_v, flags.top_v}, markers);
#ifdef THREADS_STD
  return std::make_pair(profile, trajectory);
#else
  return profile;
#endif
}

} // namespace lib7842
