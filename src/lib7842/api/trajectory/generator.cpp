#include "lib7842/api/trajectory/generator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

PiecewiseTrapezoidal Generator::generate(const Limits& limits, const Limiter& limiter,
                                         const Modifier& modifier, const Executor& executor,
                                         const Spline& spline, const QTime& dt,
                                         const ProfileFlags& flags,
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
    // limit the linear velocity according to the abilities of the robot at a spot on the spline
    k.v = std::min(k.v, limiter(t));

    // calculate and run motion along trajectory
    executor(modifier(t, k));
    rate->delayUntil(dt);

    // calculate distance traveled
    QLength d_dist = k.v * dt;
    dist += d_dist;
    // calculate where along the spline we will be at the end of the timeslice
    t = spline.t_at_dist_travelled(t, d_dist);
    // calculate new velocity
    k = profile.calc(dist);
  }
  KinematicState end = profile.end();
  if (end.v == 0_mps) { executor(modifier(1, end)); }
  return profile;
}

void SkidSteerGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
                                const std::vector<std::pair<Number, Number>>& markers) {
  auto limiter = [&](double t) { return limits.max_vel_at_curvature(spline.curvature(t)); };

  auto modifier = [&](double t, const KinematicState& k) {
    QAngularSpeed w = spline.curvature(t) * k.v * radian;
    QSpeed left = k.v - (w / radian * scales.wheelTrack) / 2;
    QSpeed right = k.v + (w / radian * scales.wheelTrack) / 2;

    QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
    QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

    auto leftSpeed = leftWheel / gearset;
    auto rightSpeed = rightWheel / gearset;

    return std::make_pair(leftSpeed, rightSpeed);
  };

  auto executor = [&](const Generator::DriveCommand& c) {
    double left = c.first.convert(number);
    double right = c.second.convert(number);
    if (forward) {
      model->tank(left, right);
    } else {
      model->tank(-right, -left);
    }
  };

  Generator::generate(limits, limiter, modifier, executor, spline, dt, flags, markers);
}

void XGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
                        const std::vector<std::pair<Number, Number>>& markers) {
  auto limiter = [&](double t) { return limits.max_vel_at_curvature(spline.curvature(t)); };

  auto modifier = [&](double t, const KinematicState& k) {
    QAngularSpeed w = spline.curvature(t) * k.v * radian;
    QSpeed left = k.v / std::sqrt(2) - (w / radian * scales.wheelTrack) / 2;
    QSpeed right = k.v / std::sqrt(2) + (w / radian * scales.wheelTrack) / 2;

    QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
    QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

    auto leftSpeed = leftWheel / gearset;
    auto rightSpeed = rightWheel / gearset;

    return std::make_pair(leftSpeed, rightSpeed);
  };

  auto executor = [&](const Generator::DriveCommand& c) {
    double left = c.first.convert(number);
    double right = c.second.convert(number);
    if (forward) {
      model->tank(left, right);
    } else {
      model->tank(-right, -left);
    }
  };

  Generator::generate(limits, limiter, modifier, executor, spline, dt, flags, markers);
}

void XGenerator::followX(const Spline& spline, bool forward, const ProfileFlags& flags,
                         const std::vector<std::pair<Number, Number>>& markers) {
  auto limiter = [&](double t) {
    auto pos = spline.calc(t);
    auto& theta = pos.theta;
    return limits.v / (sin(theta).abs() + cos(theta).abs());
  };

  auto modifier = [&](double t, const KinematicState& k) {
    auto pos = spline.calc(t);
    auto& theta = pos.theta;

    auto scaleTopLeft = sin(theta + 45_deg);
    auto scaleTopRight = sin(theta - 45_deg);

    QAngularSpeed topLeft = (k.v * scaleTopLeft / (1_pi * scales.wheelDiameter)) * 360_deg;
    QAngularSpeed topRight = (k.v * scaleTopRight / (1_pi * scales.wheelDiameter)) * 360_deg;

    auto topLeftSpeed = topLeft / gearset;
    auto topRightSpeed = topRight / gearset;

    return std::make_pair(topLeftSpeed, topRightSpeed);
  };

  auto executor = [&](const Generator::DriveCommand& c) {
    double topLeft = c.first.convert(number);
    double topRight = c.second.convert(number);

    model->getTopLeftMotor()->moveVoltage(topLeft * 12000);
    model->getTopRightMotor()->moveVoltage(topRight * 12000);
    model->getBottomLeftMotor()->moveVoltage(topRight * 12000);
    model->getBottomRightMotor()->moveVoltage(topLeft * 12000);
  };

  Generator::generate(limits, limiter, modifier, executor, spline, dt, flags, markers);
}

} // namespace lib7842