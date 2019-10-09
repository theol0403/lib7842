#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "simplePath.hpp"

#include "lib7842/point/point.hpp"

namespace lib7842 {

class ReferencePath : public AbstractPath {

 public:
  /**
   * Default Constructors
   */
  ReferencePath() = default;
  ReferencePath(const ReferencePath& ipath) = default;
  virtual ~ReferencePath() = default;

  /**
   * Explicit Constructors
   */
  explicit ReferencePath(const std::vector<PointReference>& ipath);

  /**
   * Explicit Functions
   */
  std::vector<PointReference>& get();
  operator std::vector<PointReference>&();

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

 protected:
  std::vector<PointReference> path {};
};

} // namespace lib7842