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

class StrafeGenerator {
public:
  struct Step {
    State p;
    KinematicState k;
    QSpeed p_vel;
    Number left = 0_pct;
    Number right = 0_pct;
  };

  using Runner = std::function<void(const Step&)>;

  virtual ~StrafeGenerator() = default;

  StrafeGenerator(const Limits& ilimits, const ChassisScales& iscales, const QTime& idt);

  PiecewiseTrapezoidal generate(const Spline& spline, const Runner& runner,
                                const ProfileFlags& flags = {},
                                const std::vector<std::pair<Number, Number>>& markers = {}) const;

  QSpeed maxSpeedAtTheta(const QAngle& angle) const;

protected:
  Limits limits;
  ChassisScales scales;
  QTime dt;
};

class XStrafeGenerator : public StrafeGenerator {
public:
  XStrafeGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
                   const Limits& ilimits, const ChassisScales& iscales, const QTime& idt);

  void follow(const Spline& spline, bool forward = true, const ProfileFlags& flags = {},
              const std::vector<std::pair<Number, Number>>& markers = {});

  void run(const Step& s, bool forward = true);

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  std::shared_ptr<AbstractRate> rate;
};

class XStrafeTestGenerator : public StrafeGenerator {
public:
  using StrafeGenerator::StrafeGenerator;
  std::tuple<std::vector<Step>, PiecewiseTrapezoidal>
    follow(const Spline& spline, bool /*forward*/ = true, const ProfileFlags& flags = {},
           const std::vector<std::pair<Number, Number>>& markers = {});

  void run(const Step& s);
};

} // namespace lib7842