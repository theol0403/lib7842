#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "simplePath.hpp"

#include "lib7842/point/point.hpp"
#include <variant>

namespace lib7842 {

class CompoundPath : public AbstractPath {

public:
  /**
   * Default Constructors
   */
  CompoundPath() = default;
  CompoundPath(const CompoundPath& ipath) = default;
  virtual ~CompoundPath() = default;

  /**
   * Explicit Constructors
   */
  explicit CompoundPath(const std::shared_ptr<AbstractPath>& ipath);

  /**
   * Explicit Functions
   */
  CompoundPath& addPath(const std::shared_ptr<AbstractPath>& ipath);

  CompoundPath& copyPath(const AbstractPath& ipath);
  CompoundPath& importPath(const AbstractPath& ipath);

  /**
   * Extract path of pointers to the points
   */
  virtual SimplePath extract() const override;

  /**
   * Extract path of pointers to copies of the points
   */
  virtual SimplePath extractCopy() const override;

  /**
   * @return shared pointer to copy of path
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
  std::vector<std::shared_ptr<AbstractPath>> paths {};
}; // namespace lib7842

} // namespace lib7842