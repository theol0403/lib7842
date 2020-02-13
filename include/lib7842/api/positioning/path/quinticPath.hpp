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
class QuinticPath : public TemplatePath<QuinticPath> {
public:
  /**
   * Create a spline from an array of points, containing a position and a heading.
   *
   * @param ipath        The path
   * @param islopeScalar The tuning constant for how much to smooth the corners.
   */
  explicit QuinticPath(const StatePath& ipath, double islopeScalar);

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(int isteps = 1) const override;

protected:
  StatePath path {};
  double slopeScalar {0};
};

} // namespace lib7842