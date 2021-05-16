#include "lib7842/api/trajectory/generator/skidGenerator.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Generator::Output SkidSteerGenerator::follow(const Spline& spline, bool forward,
                                             const Profile<>::Flags& flags,
                                             const PiecewiseTrapezoidal::Markers& markers) {
#ifdef THREADS_STD
  std::vector<Generator::Step> trajectory;
#endif
  if (model && flags.start_v == 0_pct) {
    model->stop();
    pros::delay(10);
  }

  auto runner = [&](double t, Profile<>::State& k) {
#ifdef THREADS_STD
    auto profiled_vel = k.v; // used for logging
#endif

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

    auto leftSpeed = Generator::toWheel(left, scales, gearset).convert(number);
    auto rightSpeed = Generator::toWheel(right, scales, gearset).convert(number);

    if (model) {
      if (forward) {
        model->left(leftSpeed);
        model->right(rightSpeed);
      } else {
        model->left(-rightSpeed);
        model->right(-leftSpeed);
      }
    }

#ifdef THREADS_STD
    trajectory.emplace_back(spline.calc(t), k, w, curvature, profiled_vel, leftSpeed, rightSpeed);
#endif
  };

  auto profile = Generator::generate(limits, runner, spline, dt, flags, markers);

#ifdef THREADS_STD
  return std::make_pair(profile, trajectory);
#else
  return profile;
#endif
}

} // namespace lib7842
