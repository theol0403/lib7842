#pragma once
#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "pursuitLimits.hpp"
#include "pursuitPath.hpp"
#include <optional>

namespace lib7842 {

class PathFollower {
public:
  PathFollower(const std::shared_ptr<ChassisModel>& imodel,
               const std::shared_ptr<Odometry>& iodometry,
               const QLength& ilookahead);

  void followPath(const PursuitPath& ipath);

protected:
  PursuitPath::array_t::const_iterator findClosest(const PursuitPath& ipath, const Vector& ipos);

  Vector findLookaheadPoint(const PursuitPath& ipath, const Vector& ipos);

  static std::optional<double> findIntersectT(const Vector& ifirst,
                                              const Vector& isecond,
                                              const Vector& ipos,
                                              const QLength& ilookahead);

  static QCurvature calculateCurvature(const State& istate, const Vector& ilookaheadPoint);

  static std::valarray<QSpeed> calculateVelocity(const QSpeed& ivel,
                                                 const QCurvature& icurvature,
                                                 const QLength& ichassisWidth);

  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};

  const QLength lookahead {0_in};

  std::optional<PursuitPath::array_t::const_iterator> lastClosest {std::nullopt};
  size_t lastLookIndex {0};
  double lastLookT {0};
};
} // namespace lib7842
