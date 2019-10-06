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
  AbstractPath() = delete;
  AbstractPath(const AbstractPath& ipath) = delete;
  virtual ~AbstractPath() = 0;

  /**
   * Extractors
   */
  virtual SimplePath extract() const = 0;
  virtual ReferencePath extractRef() const = 0;
};

} // namespace lib7842