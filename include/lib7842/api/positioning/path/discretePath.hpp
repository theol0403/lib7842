#pragma once
#include "abstractPath.hpp"

#include "lib7842/api/positioning/point/dataPoint.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <vector>

namespace lib7842 {

// forward declaration
template <typename T> class DiscretePath;
using SimplePath = DiscretePath<Vector>;

/**
 * A path that represents a collection of discrete points which are stored using shared pointers. To
 * have full access to the underlying array, use `operator()`. To have read-only access to the
 * underlying array, use `read()`.
 *
 * There are three types of discrete paths: SimplePath<Vector>, DataPath<DataPoint>, and
 * StatePath<State>>
 *
 * @tparam T The point type.
 */
template <typename T> class DiscretePath : public AbstractPath {
public:
  DiscretePath() = default;

  /**
   * Create a path using an array of points. The points will be reallocated as shared pointers.
   *
   * @param ipath The array of points
   */
  explicit DiscretePath(const std::initializer_list<T>& ipath);
  explicit DiscretePath(const std::vector<T>& ipath);

  /**
   * Create a path using an array of shared pointers.
   *
   * @param ipath The array of shared pointers
   */
  explicit DiscretePath(const std::vector<std::shared_ptr<T>>& ipath);

  /**
   * Convert a SimplePath to a Derived Path. Avoids conversion if Derived Path is already a
   * SimplePath.
   *
   * @param  ipath     The path
   */
  template <typename std::enable_if<true>* = nullptr> DiscretePath(const SimplePath& ipath) {
    path.reserve(ipath.read().size());
    std::transform(
      ipath.read().begin(), ipath.read().end(), std::back_inserter(path),
      [](const std::shared_ptr<Vector>& ipoint) {
        return std::make_shared<T>(*ipoint);
      });
  }

  /**
   * Get the underlying array.
   */
  std::vector<std::shared_ptr<T>>& get();
  std::vector<std::shared_ptr<T>>& operator()();

  /**
   * Get the underlying array, read-only.
   */
  const std::vector<std::shared_ptr<T>>& read() const;

  /**
   * Copy the entire path.
   *
   * @return copy of the path
   */
  DiscretePath<T> copy() const;

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

  std::vector<std::shared_ptr<T>> path {};
};

using DataPath = DiscretePath<DataPoint>;
using StatePath = DiscretePath<State>;

} // namespace lib7842