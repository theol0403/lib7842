#pragma once
#include "lib7842/api/positioning/point/vector.hpp"
#include <memory>

namespace lib7842 {

// forward declaration
template <typename T> class DiscretePath;
using SimplePath = DiscretePath<Vector>;

/**
 * A generic path representation that can be interpolated into a SimplePath.
 */
class AbstractPath {
public:
  AbstractPath() = default;
  virtual ~AbstractPath() = default;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  virtual SimplePath generate(int isteps = 1) const = 0;

  /**
   * Implicitly convert path to a shared pointer
   */
  virtual operator std::shared_ptr<AbstractPath>() & = 0;
  virtual operator std::shared_ptr<AbstractPath>() && = 0;
  virtual operator std::shared_ptr<AbstractPath>() const& = 0;
  virtual operator std::shared_ptr<AbstractPath>() const&& = 0;
};

template <typename Derived> class TemplatePath : public AbstractPath {
public:
  TemplatePath() = default;
  virtual ~TemplatePath() = default;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  virtual SimplePath generate(int isteps = 1) const = 0;

  /**
   * Implicitly convert path to a shared pointer
   */
  operator std::shared_ptr<AbstractPath>() & override {
    return std::make_shared<Derived>(*static_cast<Derived*>(this));
  }

  operator std::shared_ptr<AbstractPath>() && override {
    return std::make_shared<Derived>(std::move(*static_cast<Derived*>(this)));
  }

  operator std::shared_ptr<AbstractPath>() const& override {
    return std::make_shared<Derived>(*static_cast<const Derived*>(this));
  }

  operator std::shared_ptr<AbstractPath>() const&& override {
    return std::make_shared<Derived>(std::move(*static_cast<const Derived*>(this)));
  }
};
} // namespace lib7842