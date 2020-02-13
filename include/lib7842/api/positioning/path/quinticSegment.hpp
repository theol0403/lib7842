#pragma once
#include "abstractPath.hpp"
#include "discretePath.hpp"
#include "lib7842/api/positioning/point/data.hpp"

namespace lib7842 {

/**
 * A path that represents a quintic spline.
 */
class QuinticSegment : public AbstractPath<QuinticSegment> {
public:
  /**
   * Create a spline from two points, containing a position, heading, and slope.
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