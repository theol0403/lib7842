#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/spline/spline.hpp"
#include "lib7842/api/trajectory/profile/limits.hpp"
#include "lib7842/api/trajectory/profile/piecewise_trapezoidal.hpp"
#include "okapi/impl/util/rate.hpp"

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

  using Output = std::pair<PiecewiseTrapezoidal, std::vector<Step>>;
};
} // namespace lib7842