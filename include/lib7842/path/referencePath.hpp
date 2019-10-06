#pragma once
#include "main.h"
#include "abstractPath.hpp"
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

  using PointRef = std::reference_wrapper<const QPoint>;

  /**
   * Explicit Constructors
   */
  ReferencePath(const std::vector<PointRef>& ipath);

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;

  std::vector<PointRef>& get();

 protected:
  std::vector<PointRef> path {};
};

} // namespace lib7842