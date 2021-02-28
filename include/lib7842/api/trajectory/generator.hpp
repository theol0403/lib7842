#pragma once

#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/spline/spline.hpp"
#include "limits.hpp"
#include "okapi/impl/util/rate.hpp"
#include "piecewise_trapezoidal.hpp"

namespace lib7842 {

class Generator {
public:
  // wheel speed pct for left and right
  using DriveCommand = std::pair<Number, Number>;

  // return max speed as a fuction of location on the path
  using Limiter = std::function<QSpeed(double)>;

  // return motor power as a function of location on the path and profiled speed
  using Modifier = std::function<DriveCommand(double, const KinematicState&)>;

  // run or record the motor speeds
  using Executor = std::function<void(const DriveCommand&)>;

  // method that brings everything together
  static PiecewiseTrapezoidal generate(const Limits& ilimits, const Limiter& ilimiter,
                                       const Modifier& imodifier, const Executor& iexecutor,
                                       const Spline& spline, const QTime& idt = 10_ms,
                                       const ProfileFlags& flags = {},
                                       const std::vector<std::pair<Number, Number>>& markers = {});
};

class XGenerator {
public:
  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const ChassisScales& iscales, const Limits& ilimits, const QTime& idt) :
    model(std::move(imodel)), gearset(igearset), scales(iscales), limits(ilimits), dt(idt) {};

  void follow(const Spline& spline, bool forward = true, const ProfileFlags& flags = {},
              const std::vector<std::pair<Number, Number>>& markers = {});

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
};

} // namespace lib7842