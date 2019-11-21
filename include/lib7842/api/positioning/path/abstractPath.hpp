#pragma once

namespace lib7842 {

class SimplePath;

/**
 * A generic path representation that can be interpolated into a SimplePath.
 */
class AbstractPath {
public:
  virtual ~AbstractPath() = default;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment
   * @return generated path
   */
  virtual SimplePath generate(const int isteps = 1) const = 0;
};

} // namespace lib7842