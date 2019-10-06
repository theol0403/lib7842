#pragma once
#include "main.h"
#include "abstractPath.hpp"
#include "referencePath.hpp"

#include "lib7842/point/point.hpp"

namespace lib7842 {

class SimplePath : public AbstractPath {

 public:
  /**
   * Default Constructors
   */
  SimplePath() = default;
  SimplePath(const SimplePath& ipath) = default;
  virtual ~SimplePath() = default;

  /**
   * Explicit Constructors
   */
  explicit SimplePath(const std::initializer_list<QPoint>& ipath);
  explicit SimplePath(const std::vector<QPoint>& ipath);

  /**
   * Explicit Functions
   */
  std::vector<QPoint>& get();

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;

 protected:
  std::vector<QPoint> path {};
};

} // namespace lib7842