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
   * Interpolate the path
   *
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
   * @return generated path
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