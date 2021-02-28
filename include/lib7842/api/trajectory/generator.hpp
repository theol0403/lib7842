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

  // return speed and motor power as a function of location on the path and profiled speed
  using Modifier = std::function<DriveCommand(double, KinematicState&)>;

  // run or record the motor speeds
  using Executor = std::function<void(const DriveCommand&)>;

  // method that brings everything together
  static PiecewiseTrapezoidal generate(const Limits& limits, const Modifier& modifier,
                                       const Executor& executor, const Spline& spline,
                                       const QTime& dt = 10_ms, const ProfileFlags& flags = {},
                                       const std::vector<std::pair<Number, Number>>& markers = {});

  // convert wheel velocity to wheel percentage
  static Number toWheel(const QSpeed& v, const ChassisScales& scales, const QAngularSpeed& gearset);

  struct Step {
    State p;
    KinematicState k;
    QAngularSpeed w;
    QCurvature c;
    QSpeed p_vel;
    Number left {0_pct};
    Number right {0_pct};
  };

  using Output = std::pair<std::vector<Step>, PiecewiseTrapezoidal>;
};

class SkidSteerGenerator {
public:
  virtual ~SkidSteerGenerator() = default;

  SkidSteerGenerator(std::shared_ptr<ChassisModel> imodel, const QAngularSpeed& igearset,
                     const ChassisScales& iscales, const Limits& ilimits, const QTime& idt,
                     bool iisXdrive = false) :
    model(std::move(imodel)),
    gearset(igearset),
    scales(iscales),
    limits(ilimits),
    dt(idt),
    isXdrive(iisXdrive) {
    if (isXdrive) { limits.v *= std::sqrt(2); }
  };

  Generator::Output follow(const Spline& spline, bool forward = true,
                           const ProfileFlags& flags = {},
                           const std::vector<std::pair<Number, Number>>& markers = {});

  virtual void executor(const Generator::DriveCommand& c);

protected:
  std::shared_ptr<ChassisModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
  bool isXdrive;
};

class XGenerator {
public:
  virtual ~XGenerator() = default;

  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const ChassisScales& iscales, const Limits& ilimits, const QTime& idt) :
    model(std::move(imodel)), gearset(igearset), scales(iscales), limits(ilimits), dt(idt) {
    limits.v *= std::sqrt(2);
  };

  Generator::Output follow(const Spline& spline, const ProfileFlags& flags = {},
                           const std::vector<std::pair<Number, Number>>& markers = {});

  virtual void executor(const Generator::DriveCommand& c);

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
};

} // namespace lib7842