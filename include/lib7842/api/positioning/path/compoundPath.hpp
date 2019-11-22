#pragma once
#include "abstractPath.hpp"
#include "discretePath.hpp"

namespace lib7842 {

/**
 * A path that represents a combination of segments that can be interpolated into a SimplePath.
 */
class CompoundPath : public AbstractPath {
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
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

  /**
   * Implictly convert path to a shared pointer
   */
  operator std::shared_ptr<AbstractPath>() & override;
  operator std::shared_ptr<AbstractPath>() && override;

protected:
  std::vector<std::shared_ptr<AbstractPath>> segments {};
};

} // namespace lib7842