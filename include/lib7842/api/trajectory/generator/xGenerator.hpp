#pragma once
#include "generator.hpp"

namespace lib7842 {

using Rotator = std::function<QAngularSpeed(const KinematicState&)>;

constexpr auto makeRotator(const QAngularSpeed& speed) {
  return [=](const KinematicState& /*ignore*/) { return speed; };
}

constexpr auto makeRotator(const QTime& time, const QAngularSpeed& speed,
                           const QAngularSpeed& maxSpeed = 1000_rpm) {
  return [=](const KinematicState& k) {
    return std::min(maxSpeed, k.t < time / 2 ? speed * k.t / (time / 2)
                                             : speed * (k.t - k.t / 2) / (time / 2));
  };
}

struct xMovement {
  bool curvature {false};
  Rotator rotator {makeRotator(0_rpm)};
  QAngle start {0_deg};
};

class XGenerator {
public:
  virtual ~XGenerator() = default;

  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const ChassisScales& iscales, const Limits& ilimits, const QTime& idt) :
    model(std::move(imodel)), gearset(igearset), scales(iscales), limits(ilimits), dt(idt) {
    limits.v *= std::sqrt(2);
    limits.a *= std::sqrt(2);
  };

  Generator::Output follow(const Spline& spline, const xMovement& movement = {},
                           const ProfileFlags& flags = {},
                           const PiecewiseTrapezoidal::Markers& markers = {});

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
};

} // namespace lib7842
