#pragma once
#include "simplePath.hpp"

#include "lib7842/api/positioning/point/dataPoint.hpp"

namespace lib7842 {

/**
 * A path that represents a collection of data points which are stored using shared pointers. To have
 * full access to the underlying array, use `operator()`. To have read-only access to the
 * underlying array, use `read()`.
 */
class DataPath : public AbstractPath {
public:
  DataPath() = default;

  /**
   * Create a path using an array of data points. The points will be reallocated as shared pointers.
   *
   * @param ipath The array of data points
   */
  explicit DataPath(const std::initializer_list<DataPoint>& ipath);
  explicit DataPath(const std::vector<DataPoint>& ipath);

  /**
   * Create a path using an array of shared pointers.
   *
   * @param ipath The array of shared pointers
   */
  explicit DataPath(const std::vector<std::shared_ptr<DataPoint>>& ipath);

  /**
   * Convert a SimplePath to a DataPath
   *
   * @param ipath The path
   */
  explicit DataPath(const SimplePath& ipath);

  /**
   * Get the underlying array.
   */
  std::vector<std::shared_ptr<DataPoint>>& get();
  std::vector<std::shared_ptr<DataPoint>>& operator()();

  /**
   * Get the underlying array, read-only.
   */
  const std::vector<std::shared_ptr<DataPoint>>& read() const;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

protected:
  std::vector<std::shared_ptr<DataPoint>> path {};
};

} // namespace lib7842