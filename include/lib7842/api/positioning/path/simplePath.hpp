#pragma once
#include "abstractPath.hpp"

#include "lib7842/api/positioning/point/vector.hpp"
#include <initializer_list>
#include <memory>
#include <vector>

namespace lib7842 {

/**
 * A path that represents a collection of points which are stored using shared pointers. To have
 * full access to the underlying array, use `operator()`. To have read-only access to the
 * underlying array, use `read()`.
 */
class SimplePath : public AbstractPath {
public:
  SimplePath() = default;

  /**
   * Create a path using an array of points. The points will be reallocated as shared pointers.
   *
   * @param ipath The array of points
   */
  explicit SimplePath(const std::initializer_list<Vector>& ipath);
  explicit SimplePath(const std::vector<Vector>& ipath);

  /**
   * Create a path using an array of shared pointers.
   *
   * @param ipath The array of shared pointers
   */
  explicit SimplePath(const std::vector<std::shared_ptr<Vector>>& ipath);

  /**
   * Get the underlying array.
   */
  std::vector<std::shared_ptr<Vector>>& get();
  std::vector<std::shared_ptr<Vector>>& operator()();

  /**
   * Get the underlying array, read-only.
   */
  const std::vector<std::shared_ptr<Vector>>& read() const;

  /**
   * Copy the entire path.
   *
   * @return copy of the path
   */
  SimplePath copy() const;

  /**
   * Smoothen the path
   *
   * @param iweight    The smooth weight
   * @param itolerance The smooth tolerance
   */
  void smoothen(const double iweight, const QLength& itolerance);

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

protected:
  /**
   * Sample the segment
   *
   * @param  start  The start point
   * @param  end    The end point
   * @param  isteps the number of points to generate in the segment excluding the end
   * @return the generated segment
   */
  static SimplePath generateSegment(const Vector& start, const Vector& end, const int isteps);

  std::vector<std::shared_ptr<Vector>> path {};
};

} // namespace lib7842