#pragma once
#include "main.h"

#include "lib7842/path/simplePath.hpp"
#include "lib7842/path/packedPath.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

/**
 * Interface Generator Class
 */
class AbstractGenerator {

 public:
  /**
   * Constructors
   */
  AbstractGenerator() = default;
  AbstractGenerator(const AbstractGenerator& igenerator) = default;
  virtual ~AbstractGenerator() = default;

  /**
   * Extractors
   */
  virtual SimplePath generate(const QLength& ispacing) const = 0;
};

} // namespace lib7842