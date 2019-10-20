#pragma once
#include "main.h"

namespace lib7842 {

// Forward Declare
class SimplePath;
class ReferencePath;

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
   * Sample the path and return a path with higher resolution
   * @param  isteps the number of points to generate for each segment
   */
  virtual SimplePath generate(const int isteps = 1) const = 0;

  /**
   * Return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const = 0;

  /**
   * Move the path into a shared pointer and return pointer
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const = 0;
};

} // namespace lib7842