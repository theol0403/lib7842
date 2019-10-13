#pragma once
#include "main.h"
#include "simplePath.hpp"

#include "lib7842/point/packedPoint.hpp"

namespace lib7842 {

class PackedPath : public AbstractPath {

public:
  /**
   * Default Constructors
   */
  PackedPath() = default;
  PackedPath(const PackedPath& ipath) = default;
  virtual ~PackedPath() = default;

  /**
   * Explicit Constructors
   */
  explicit PackedPath(const std::initializer_list<PackedPoint>& ipath);
  explicit PackedPath(const std::vector<PackedPoint>& ipath);
  explicit PackedPath(const AbstractPath& ipath);

  /**
   * Explicit Functions
   */
  std::vector<PackedPoint>& get();
  std::vector<PackedPoint>& operator()();

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

private:
  // delegating constructor
  explicit PackedPath(const std::vector<PointReference>& ipath);

protected:
  std::vector<PackedPoint> path {};
};

} // namespace lib7842