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
   * Extractors
   */
  virtual SimplePath extract() const = 0;
  virtual ReferencePath extractRef() const = 0;
  virtual std::unique_ptr<AbstractPath> copyPtr() const = 0;
};

} // namespace lib7842