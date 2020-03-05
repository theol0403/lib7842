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
   * @param  isteps How many points to interpolate per segment, counting from the start to just
   *                before the end of the segment. This means is 1 step will return the first point
   *                and 2 steps will return the first point as well as a midway point. The end point
   *                is not included in the count.
   * @param  iend   Whether to return the end of the segment. This can be turned off to prevent the
   *                start of the next segment from being redundant.
   * @return generated path
   */
  virtual SimplePath generate(int isteps = 1, bool iend = true) const = 0;

  /**
   * Implicitly convert path to a shared pointer
   */
  virtual operator std::shared_ptr<AbstractPath>() & = 0;
  virtual operator std::shared_ptr<AbstractPath>() && = 0;
};

/**
 * A helper class to automatically generate the required pointer conversions for each path
 * implementation. Uses CRTP to generate the conversions. To use, inherit from this class while
 * passing the derived class as the template parameter.
 *
 * @tparam CRTP The derived class
 */
template <typename CRTP> class PathHelper : public AbstractPath {
public:
  using AbstractPath::AbstractPath;

  /**
   * Implicitly convert path to a shared pointer
   */
  operator std::shared_ptr<AbstractPath>() & override {
    return std::make_shared<CRTP>(*static_cast<CRTP*>(this));
  }

  operator std::shared_ptr<AbstractPath>() && override {
    return std::make_shared<CRTP>(std::move(*static_cast<CRTP*>(this)));
  }
};
} // namespace lib7842