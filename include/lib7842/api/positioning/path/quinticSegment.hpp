#pragma once
#include "abstractPath.hpp"
#include "lib7842/api/positioning/point/dataState.hpp"
#include <vector>

namespace lib7842 {

/**
 * A path that represents a quintic spline.
 */
class QuinticSegment : public AbstractPath {
public:
  /**
   * Create a spline from an array of points, containing a position, heading, and velocity.
   *
   * @param ipoints The points
   */
  explicit QuinticSegment(const std::vector<DataState>& ipoints);

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(int isteps = 1) const override;

protected:
  std::vector<DataState> points {};
};

} // namespace lib7842