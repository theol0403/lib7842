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
   * Extract path of pointers to the points
   */
  virtual SimplePath extract() const = 0;

  /**
   * Extract path of pointers to copies of the points
   */
  virtual SimplePath extractCopy() const = 0;

  /**
   * Extract path of references to points
   */
  virtual ReferencePath extractRef() const = 0;

  /**
   * @return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const = 0;

  /**
   * Move the path into a shared pointer and return pointer
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const = 0;

  /**
   * Sample the path and return a path with higher resolution
   * @param  isteps the number of points to generate for each segment
   */
  virtual SimplePath generate(const size_t isteps) const = 0;
};

} // namespace lib7842