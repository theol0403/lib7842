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
  // execute trajectory as a function of location on the path and profiled speed
  using Runner = std::function<void(double, Profile<>::State&)>;

  // method that brings everything together
  static PiecewiseTrapezoidal generate(const Limits<>& limits, const Runner& runner,
                                       const Spline& spline, const QTime& dt = 10_ms,
                                       const Profile<>::Flags& flags = {},
                                       const PiecewiseTrapezoidal::Markers& markers = {});

  // convert wheel velocity to wheel percentage
  static Number toWheel(const QSpeed& v, const ChassisScales& scales, const QAngularSpeed& gearset);

  struct Step {
    State p;
    Profile<>::State k;
    QAngularSpeed w;
    QCurvature c;
    QSpeed p_vel;
    double left {0};
    double right {0};
    double leftBack {0};
    double rightBack {0};
  };

#ifdef THREADS_STD
  using Output = std::pair<PiecewiseTrapezoidal, std::vector<Step>>;
#else
  using Output = PiecewiseTrapezoidal;
#endif
};
} // namespace lib7842
