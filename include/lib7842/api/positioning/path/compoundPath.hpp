#pragma once
#include "abstractPath.hpp"
#include "discretePath.hpp"

namespace lib7842 {

/**
 * A path that represents a combination of segments that can be interpolated into a SimplePath.
 */
class CompoundPath : public TemplatePath<CompoundPath> {
public:
  CompoundPath() = default;

  /**
   * Add a path segment to an existing path.
   *
   * @param  isegment The segment.
   */
  CompoundPath& add(const std::shared_ptr<AbstractPath>& isegment);
  CompoundPath& operator+=(const std::shared_ptr<AbstractPath>& isegment);

  /**
   * Combine two paths together
   *
   * @param  isegment The segment
   */
  CompoundPath operator+(const std::shared_ptr<AbstractPath>& isegment) &;
  CompoundPath&& operator+(const std::shared_ptr<AbstractPath>& isegment) &&;

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
  std::vector<std::shared_ptr<AbstractPath>> segments {};
};
} // namespace lib7842