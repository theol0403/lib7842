#pragma once
#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include <optional>

namespace lib7842 {

class PathFollower {
public:
  PathFollower(const std::shared_ptr<ChassisModel>& imodel,
               const std::shared_ptr<Odometry>& iodometry,
               const QLength& ilookahead,
               const QSpeed& iminVel);

  void followPath(const DataPath& ipath);

protected:
  DataPath::array_t::iterator findClosest(const DataPath& ipath, const Vector& ipos);

  Vector findLookaheadPoint(const Vector& ipos);

  static std::optional<double> findIntersectT(const Vector& ifirst,
                                              const Vector& isecond,
                                              const Vector& ipos,
                                              const QLength& ilookahead);

  static QCurvature calculateCurvature(const State& istate, const Vector& ilookaheadPoint);

  static std::valarray<double> calculateVelocity(double ivel, const QCurvature& icurvature);

  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};
  QLength lookahead {0_in};
  QSpeed minVel {0_mps};

  DataPath::array_t::iterator lastClosest {nullptr};
  size_t lastLookIndex {0};
  double lastLookT {0};
};
} // namespace lib7842
