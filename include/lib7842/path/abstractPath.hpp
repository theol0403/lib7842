#pragma once
#include "main.h"

namespace lib7842 {

// Forward Declare
class SimplePath;

/**
 * Interface Path Class
 */
class AbstractPath {

public:
  /**
   * Default Constructors
   */
  AbstractPath() = default;
  AbstractPath(const AbstractPath& ipath) = default;
  virtual ~AbstractPath() = default;

  /**
   * Extract path of pointers to the points
   * @return SimplePath
   */
  virtual SimplePath extract() const = 0;

  /**
   * Extract path of pointers to copies of the points
   * @return SimplePath
   */
  virtual SimplePath extractCopy() const = 0;

  /**
   * Make a copy of the path and return shared pointer
   * @return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const = 0;

  /**
   * Move the path into a shared pointer
   * @return shared pointer containing path
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const = 0;
};

} // namespace lib7842