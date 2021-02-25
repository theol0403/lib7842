#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/spline/spline.hpp"
#include "limits.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/impl/util/rate.hpp"
#include "piecewise_trapezoidal.hpp"
#include "trapezoidal.hpp"

namespace lib7842 {

class Generator {
public:
  struct Step {
    State p;
    KinematicState k;
    QAngularSpeed w;
    QCurvature c;
    QSpeed p_vel;
    Number left {0_pct};
    Number right {0_pct};
  };

  using Runner = std::function<void(const Step&)>;

  virtual ~Generator() = default;

  Generator(const Limits& ilimits, const ChassisScales& iscales, const QTime& idt);

  PiecewiseTrapezoidal generate(const Spline& spline, const Runner& runner,
                                const ProfileFlags& flags = {},
                                const std::vector<std::pair<Number, Number>>& markers = {}) const;

protected:
  Limits limits;
  ChassisScales scales;
  QTime dt;
};

class XGenerator : public Generator {
public:
  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const Limits& ilimits, const ChassisScales& iscales, const QTime& idt);

  void follow(const Spline& spline, bool forward = true, const ProfileFlags& flags = {},
              const std::vector<std::pair<Number, Number>>& markers = {});

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
};

class XTestGenerator : public Generator {
public:
  using Generator::Generator;
  std::tuple<std::vector<Step>, PiecewiseTrapezoidal>
    follow(const Spline& spline, bool /*forward*/ = true, const ProfileFlags& flags = {},
           const std::vector<std::pair<Number, Number>>& markers = {});
};

} // namespace lib7842