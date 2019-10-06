#pragma once
#include "main.h"
#include "abstractPath.hpp"
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
  explicit CompoundPath(std::unique_ptr<AbstractPath> ipath);

  /**
   * Explicit Functions
   */
  CompoundPath& addPoint(const QPoint& ipoint);
  CompoundPath& addPath(std::unique_ptr<AbstractPath> ipath);

  CompoundPath& addPoints(const std::vector<QPoint>& ipoints);
  // CompoundPath& addPaths(const std::initializer_list<std::unique_ptr<AbstractPath>>& ipaths);

  /**
   * Extractors
   */
  virtual SimplePath extract() const override;
  virtual ReferencePath extractRef() const override;

 private:
  /**
   * Wrapper Class to provide fake copy to unique_ptr
   */
  class AbstractPathPtr {
   public:
    AbstractPathPtr(std::unique_ptr<AbstractPath> ipath) : path(std::move(ipath)) {}
    AbstractPathPtr(const AbstractPathPtr&) {} // empty constructor
    const std::unique_ptr<AbstractPath>& get() const {
      return path;
    };

   protected:
    std::unique_ptr<AbstractPath> path;
  };

 protected:
  std::vector<std::variant<QPoint, AbstractPathPtr>> path;
}; // namespace lib7842

} // namespace lib7842