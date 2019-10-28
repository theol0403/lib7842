#pragma once
#include "main.h"
#include "abstractPath.hpp"

#include "lib7842/positioning/point/vector.hpp"

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
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

  /**
   * Return shared pointer to copy of path
   */
  std::shared_ptr<AbstractPath> copyPtr() const override;

  /**
   * Move the path into a shared pointer and return pointer
   */
  std::shared_ptr<AbstractPath> movePtr() const override;

protected:
  /**
   * Sample the segment
   * @param  isteps the number of points to generate in the segment excluding the end
   */
  static SimplePath generateSegment(const Vector& start, const Vector& end, const int isteps);

  std::vector<std::shared_ptr<Vector>> path {};
};

} // namespace lib7842