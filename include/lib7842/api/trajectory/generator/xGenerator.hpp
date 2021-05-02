#pragma once
#include "generator.hpp"

namespace lib7842 {

using Rotator = std::function<QAngularSpeed(const Profile<>::State&)>;

constexpr auto makeRotator(const QAngularSpeed& speed) {
  return [=](const Profile<>::State& /*ignore*/) { return speed; };
}

inline auto makeRotator(const QAngle& angle, const Limits<QAngle>& limits) {
  Trapezoidal<QAngle> profile(limits, angle);
  return [=](const Profile<>::State& k) { return profile.calc(k.t).v; };
}

struct XFlags : public Profile<>::Flags {
  bool curve {false};
  Rotator rotator {makeRotator(0_rpm)};
  QAngle start {0_deg};
};

class XGenerator {
public:
  virtual ~XGenerator() = default;

  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const ChassisScales& iscales, const Limits<>& ilimits, const QTime& idt) :
    model(std::move(imodel)), gearset(igearset), scales(iscales), limits(ilimits), dt(idt) {
    limits.v *= std::sqrt(2);
    limits.a *= std::sqrt(2);
  };

  Generator::Output follow(const Spline& spline, const XFlags& flags = {},
                           const PiecewiseTrapezoidal::Markers& markers = {});

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits<> limits;
  QTime dt;
};

} // namespace lib7842
