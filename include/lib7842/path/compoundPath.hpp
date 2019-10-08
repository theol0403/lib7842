#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "simplePath.hpp"
#include "referencePath.hpp"

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
  explicit CompoundPath(const AbstractPath& ipath);
  explicit CompoundPath(const std::shared_ptr<AbstractPath>& ipath);

  /**
   * Explicit Functions
   */
  CompoundPath& addPath(const AbstractPath& ipath);
  CompoundPath& addPath(const std::shared_ptr<AbstractPath>& ipath);

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;

  // protected:
  std::vector<std::shared_ptr<AbstractPath>> paths {};
}; // namespace lib7842

} // namespace lib7842