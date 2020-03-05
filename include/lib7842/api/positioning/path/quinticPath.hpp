#pragma once
#include "abstractPath.hpp"
#include "discretePath.hpp"
#include "lib7842/api/positioning/point/data.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include <vector>

namespace lib7842 {

/**
 * A path that represents a quintic spline.
 */
class QuinticPath : public PathHelper<QuinticPath> {
public:
  /**
   * Create a spline from an array of points, each containing a position and an angle.
   *
   * @param ipath        The path
   * @param islopeScalar The importance of each point's angle.
   */
  explicit QuinticPath(const StatePath& ipath, double islopeScalar);

  /**
   * Create a spline from an array of points containing only positions. The angle of each point is
   * guessed from the positioning of nearby points.
   *
   * @param ipath        The path
   * @param islopeScalar The importance of each point's angle.
   */
  explicit QuinticPath(const SimplePath& ipath, double islopeScalar);

  /**
   * Interpolate the path
   *
   * @param  isteps How many points to interpolate per segment, counting from the start to just
   *                before the end of the segment. This means is 1 step will return the first point
   *                and 2 steps will return the first point as well as a midway point. The end point
   *                is not included in the count.
   * @param  iend   Whether to return the end of the segment. This can be turned off to prevent the
   *                start of the next segment from being redundant.
   * @return generated path
   */
  SimplePath generate(int isteps = 1, bool iend = true) const override;

protected:
  StatePath path {};
  double slopeScalar {0};
};

} // namespace lib7842