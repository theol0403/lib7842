#pragma once
#include "lib7842/api/positioning/point/vector.hpp"

namespace lib7842 {

// forward declaration
template <typename T> class DiscretePath;
using SimplePath = DiscretePath<Vector>;

/**
 * A generic path representation that can be interpolated into a SimplePath.
 */
class AbstractPath {
public:
  AbstractPath() = default;
  AbstractPath(const AbstractPath& ipath) = default;
  AbstractPath(AbstractPath&& ipath) = default;
  virtual ~AbstractPath() = default;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  virtual SimplePath generate(const int isteps = 1) const = 0;
};

} // namespace lib7842