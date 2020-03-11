#include "pathFollower.hpp"

namespace lib7842 {

class PathFollowerX : public PathFollower {
public:
  /**
   * PathFollowerX
   *
   * @param imodel         The chassis model
   * @param iodometry      The odometry
   * @param ichassisScales The chassis scales
   * @param ilookahead     The lookahead distance
   */
  PathFollowerX(const std::shared_ptr<ChassisModel>& imodel,
                const std::shared_ptr<Odometry>& iodometry, const ChassisScales& ichassisScales,
                const QLength& ilookahead);

  /**
   * Follow a pre-generated PursuitPath using holonomic control, where heading is independently controlled.
   *
   * @param ipath      The path
   */
  void followPath(const PursuitPath& ipath);
};

} // namespace lib7842