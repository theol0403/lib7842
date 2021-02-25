#include "lib7842/api/trajectory/xGenerator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

StrafeGenerator::StrafeGenerator(const Limits& ilimits, const ChassisScales& iscales,
                                 const QTime& idt) :
  limits(ilimits), scales(iscales), dt(idt) {}

PiecewiseTrapezoidal
  StrafeGenerator::generate(const Spline& spline, const Runner& runner, const ProfileFlags& flags,
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
    QSpeed vel_max = std::min(k.v, maxSpeedAtTheta(pos.theta));
    // project where along the spline we will be after dt, given approximate velocity
    double t_n = spline.t_at_dist_travelled(t, vel_max * dt);
    State pos_new = spline.calc(t_n);

    // limit profiled velocity to angle
    k.v = std::min(k.v, maxSpeedAtTheta(pos_new.theta));

    // calculate distance traveled
    QLength d_dist = k.v * dt;
    dist += d_dist;
    // calculate where along the spline we will be at the end of the timeslice
    t = spline.t_at_dist_travelled(t, d_dist);

    // run trajectory
    runner({pos, k, profiled_vel});

    // update new position
    pos = spline.calc(t);
    // calculate new velocity
    k = profile.calc(dist);
  }
  KinematicState end = profile.end();
  if (end.v == 0_mps) { runner({pos, end, end.v}); }
  return profile;
}

QSpeed StrafeGenerator::maxSpeedAtTheta(const QAngle& angle) const {
  auto rot = angle + 45_deg;
  return limits.v * (sin(rot) + cos(rot)) / std::sqrt(2);
}

XStrafeGenerator::XStrafeGenerator(std::shared_ptr<XDriveModel> imodel,
                                   const QAngularSpeed& igearset, const Limits& ilimits,
                                   const ChassisScales& iscales, const QTime& idt) :
  StrafeGenerator(ilimits, iscales, idt),
  model(std::move(imodel)),
  gearset(igearset),
  rate(std::shared_ptr<AbstractRate>(global::getTimeUtil()->getRate())) {
  {}
}

void XStrafeGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
                              const std::vector<std::pair<Number, Number>>& markers) {
  generate(
    spline, [this, forward](const Step& s) { run(s, forward); }, flags, markers);
}

void XStrafeGenerator::run(const StrafeGenerator::Step& s, bool forward) {
  QSpeed outSpeed = s.k.v * maxSpeedAtTheta(s.p.theta) / limits.v;

  auto wheelDirection = (s.p.theta + 45_deg).convert(radian);
  auto scaleTopLeft = sin(wheelDirection);
  auto scaleTopRight = cos(wheelDirection);

  QAngularSpeed topLeft = (outSpeed * scaleTopLeft / (1_pi * scales.wheelDiameter)) * 360_deg;
  QAngularSpeed topRight = (outSpeed * scaleTopRight / (1_pi * scales.wheelDiameter)) * 360_deg;

  auto topLeftSpeed = (topLeft / gearset).convert(number);
  auto topRightSpeed = (topRight / gearset).convert(number);

  model->getTopLeftMotor()->moveVoltage(topLeftSpeed * 12000);
  model->getTopRightMotor()->moveVoltage(topRightSpeed * 12000);
  model->getBottomLeftMotor()->moveVoltage(topRightSpeed * 12000);
  model->getBottomRightMotor()->moveVoltage(topLeftSpeed * 12000);
  rate->delayUntil(dt);
}

std::tuple<std::vector<StrafeGenerator::Step>, PiecewiseTrapezoidal>
  XStrafeTestGenerator::follow(const Spline& spline, bool /*forward*/, const ProfileFlags& flags,
                               const std::vector<std::pair<Number, Number>>& markers) {
  std::vector<Step> trajectory;
  auto profile = generate(
    spline,
    [&](const Step& s) {
      QSpeed outSpeed = s.k.v * maxSpeedAtTheta(s.p.theta) / limits.v;

      auto wheelDirection = (s.p.theta + 45_deg).convert(radian);
      auto scaleTopLeft = sin(wheelDirection);
      auto scaleTopRight = cos(wheelDirection);

      QAngularSpeed topLeft = (outSpeed * scaleTopLeft / (1_pi * scales.wheelDiameter)) * 360_deg;
      QAngularSpeed topRight = (outSpeed * scaleTopRight / (1_pi * scales.wheelDiameter)) * 360_deg;

      auto topLeftSpeed = (topLeft / 200_rpm);
      auto topRightSpeed = (topRight / 200_rpm);

      Step ns = s;
      ns.left = topLeftSpeed;
      ns.right = topRightSpeed;
      trajectory.emplace_back(ns);
    },
    flags, markers);
  return {trajectory, profile};
}

} // namespace lib7842