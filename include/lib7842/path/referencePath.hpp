#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "simplePath.hpp"

#include "lib7842/point/point.hpp"

namespace lib7842 {

class ReferencePath : public AbstractPath {

 public:
  /**
   * Custom Types
   */
  using PointRef = std::reference_wrapper<const QPoint>;

  /**
   * Default Constructors
   */
  ReferencePath() = default;
  ReferencePath(const ReferencePath& ipath) = default;
  virtual ~ReferencePath() = default;

  /**
   * Explicit Constructors
   */
  explicit ReferencePath(const std::vector<PointRef>& ipath);

  /**
   * Explicit Functions
   */
  std::vector<PointRef>& get();

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

 protected:
  std::vector<PointRef> path {};
};

} // namespace lib7842