#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "simplePath.hpp"

#include "lib7842/point/point.hpp"

namespace lib7842 {

class CompoundPath : public AbstractPath {

public:
  /**
   * Default Constructors
   */
  CompoundPath() = default;
  CompoundPath(const CompoundPath& ipath) = default;
  virtual ~CompoundPath() = default;

  explicit CompoundPath(const std::shared_ptr<AbstractPath>& ipath);

  CompoundPath& addPath(const std::shared_ptr<AbstractPath>& ipath);

  CompoundPath& copyPath(const AbstractPath& ipath);
  CompoundPath& importPath(const AbstractPath& ipath);

  /**
   * Sample the path and return a path with higher resolution
   * @param  isteps the number of points to generate for each segment
   */
  virtual SimplePath generate(const size_t isteps = 1) const override;

  /**
   * Return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;

  /**
   * Move the path into a shared pointer and return pointer
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

protected:
  std::vector<std::shared_ptr<AbstractPath>> paths {};
}; // namespace lib7842

} // namespace lib7842