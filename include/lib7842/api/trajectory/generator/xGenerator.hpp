#pragma once
#include "generator.hpp"
#include "lib7842/api/other/utility.hpp"
#include <optional>

namespace lib7842 {

// an angler is a function that accepts a state and returns an angular velocity
using Angler = std::function<QAngularSpeed(const Profile<>::State&)>;

// make an angler that returns a constant velocity
constexpr auto makeAngler(const QAngularSpeed& speed) {
  return [=](const Profile<>::State& /*ignore*/) { return speed; };
}

// make an angler that returns a trapezoidal velocity
inline auto makeAngler(const QAngle& angle, const Limits<QAngle>& limits) {
  Trapezoidal<QAngle> profile(limits, angle.abs());
  return [=](const Profile<>::State& k) { return util::sgn(angle) * profile.calc(k.t).v; };
}

// flags that can be used to control the motion
// it is indented to be used with C++20 desingated initializers
struct XFlags {
  Number start_v = 0_pct; // the starting velocity percentage
  Number end_v = 0_pct; // the ending velocity percentage
  Number top_v = 100_pct; // the maximum velocity percentage
  bool curve {false}; // whether to follow the curve by steering or stafing
  std::optional<QAngle> start {std::nullopt}; // the angle at which the robot starts at. Defaults to
                                              // the starting angle of the path.
  Angler rotator {makeAngler(0_rpm)}; // uses an angler to give the robot an angular velocity
                                      // without shifting the robot's frame of reference
  Angler steerer {
    makeAngler(0_rpm)}; // uses an angler to steer the robot, shifting its frame of reference
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
