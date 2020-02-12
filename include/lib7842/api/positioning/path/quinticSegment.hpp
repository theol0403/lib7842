#pragma once
#include "abstractPath.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/dataState.hpp"
#include <vector>

namespace lib7842 {

/**
 * A path that represents a quintic spline.
 */
class QuinticSegment : public AbstractPath {
public:
  /**
   * Create a spline from two points, containing a position, heading, and heading importance.
   *
   * @param istart The start point
   * @param iend   The end point
   */
  QuinticSegment(const DataState& istart, const DataState& iend);

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(int isteps = 1) const override;

protected:
  DataState start {};
  DataState end {};
};

} // namespace lib7842