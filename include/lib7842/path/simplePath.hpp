#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "referencePath.hpp"

#include "lib7842/point/point.hpp"

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
  explicit SimplePath(const std::vector<VectorRef>& ipath);
  explicit SimplePath(const std::vector<std::shared_ptr<Vector>>& ipath);

  std::vector<std::shared_ptr<Vector>>& get();
  std::vector<std::shared_ptr<Vector>>& operator()();

  /**
   * Extract path containing copies of points
   */
  SimplePath copy() const;

  /**
   * Extract path of references to points
   */
  ReferencePath ref() const;

  /**
   * Smoothen path
   */
  void smooth(const double& iweight, const QLength& itolerance);

  /**
   * Extract path of shared pointers
   */
  virtual SimplePath extract() const override;

  /**
   * Return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;

  /**
   * Move the path into a shared pointer and return pointer
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

  /**
   * Sample the path and return a path with higher resolution
   * @param  isteps the number of points to generate for each segment
   */
  virtual SimplePath generate(const size_t isteps) const override;

protected:
  /**
   * Sample the segment
   * @param  isteps the number of points to generate in the segment
   */
  static SimplePath generateSegment(const Vector& start, const Vector& end, const size_t isteps);

  /**
   * Generate smooth path
   */
  static SimplePath
    generateSmoothPath(const SimplePath& ipath, const double& iweight, const QLength& itolerance);

  std::vector<std::shared_ptr<Vector>> path {};
};

} // namespace lib7842