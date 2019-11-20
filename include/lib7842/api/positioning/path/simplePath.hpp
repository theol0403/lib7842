#pragma once
#include "abstractPath.hpp"

#include "lib7842/api/positioning/point/vector.hpp"
#include <initializer_list>
#include <memory>
#include <vector>

namespace lib7842 {

class SimplePath : public AbstractPath {
public:
  /**
   * Default Constructors
   */
  SimplePath() = default;
  SimplePath(const SimplePath& ipath) = default;
  virtual ~SimplePath() = default;

  explicit SimplePath(const std::initializer_list<Vector>& ipath);
  explicit SimplePath(const std::vector<Vector>& ipath);
  explicit SimplePath(const std::vector<std::shared_ptr<Vector>>& ipath);

  std::vector<std::shared_ptr<Vector>>& get();
  std::vector<std::shared_ptr<Vector>>& operator()();
  const std::vector<std::shared_ptr<Vector>>& read() const;

  /**
   * Extract path containing copies of points
   */
  SimplePath copy() const;

  /**
   * Smoothen path
   */
  void smoothen(const double iweight, const QLength& itolerance);

  /**
   * Interpolate the path
   *
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end
   *               (exclusive) of segment
   *
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

protected:
  /**
   * Sample the segment
   *
   * @param start  The start point
   * @param end    The end point
   * @param isteps the number of points to generate in the segment excluding the end
   *
   * @return { description_of_the_return_value }
   */
  static SimplePath generateSegment(const Vector& start, const Vector& end, const int isteps);

  std::vector<std::shared_ptr<Vector>> path {};
};

} // namespace lib7842