#pragma once
#include "abstractPath.hpp"

#include "lib7842/api/positioning/point/dataPoint.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <algorithm>
#include <initializer_list>
#include <vector>

namespace lib7842 {

// forward declaration
template <typename T> class DiscretePath;
using SimplePath = DiscretePath<Vector>;

/**
 * A path that represents a collection of discrete points which are stored using shared pointers. To
 * have full access to the underlying array, use `operator()`. To have read-only access to the
 * underlying array, use `read()`.
 *
 * There are three types of discrete paths: SimplePath<Vector>, DataPath<DataPoint>, and
 * StatePath<State>>. The point type must be derived from Vector.
 *
 * @tparam T The point type.
 */
template <typename T> class DiscretePath : public AbstractPath {
public:
  DiscretePath() = default;

  /**
   * Create a path using an array of points. The points will be reallocated as shared pointers.
   *
   * @param ipath The array of points
   */
  explicit DiscretePath(const std::vector<T>& ipath) {
    path.reserve(ipath.size());
    std::transform(ipath.begin(), ipath.end(), std::back_inserter(path),
                   [](const auto& ipoint) { return std::make_shared<T>(ipoint); });
  }

  explicit DiscretePath(std::vector<T>&& ipath) {
    path.reserve(ipath.size());
    std::transform(ipath.begin(), ipath.end(), std::back_inserter(path),
                   [](auto&& ipoint) { return std::make_shared<T>(std::move(ipoint)); });
  }

  /**
   * Create a path using an array of shared pointers.
   *
   * @param ipath The array of shared pointers
   */
  explicit DiscretePath(const std::vector<std::shared_ptr<T>>& ipath) : path(ipath) {}

  /**
   * Get the underlying array.
   */
  std::vector<std::shared_ptr<T>>& get() {
    return path;
  }

  std::vector<std::shared_ptr<T>>& operator()() {
    return path;
  }

  /**
   * Get the underlying array, read-only.
   */
  const std::vector<std::shared_ptr<T>>& read() const {
    return path;
  }

  /**
   * Copy the entire path.
   *
   * @return copy of the path
   */
  DiscretePath<T> copy() const {
    DiscretePath<T> temp;
    temp().reserve(path.size());
    for (auto&& point : path) {
      temp().emplace_back(std::make_shared<T>(*point));
    }
    return temp;
  }

  /**
   * Smoothen the path
   *
   * @param iweight    The smooth weight
   * @param itolerance The smooth tolerance
   */
  void smoothen(const double iweight, const QLength& itolerance) {
    DiscretePath<T> destPath = copy();

    double weight = 1.0 - iweight;
    QLength change = itolerance;

    while (change >= itolerance) {
      change = 0.0_in;
      for (size_t i = 1; i < path.size() - 1; i++) {
        for (size_t j = 0; j < 2; j++) {
          QLength& destPoint = destPath()[i]->at(j);
          QLength dataFac = iweight * (path[i]->read(j) - destPoint);
          QLength smoothFac =
            weight * (destPath()[i - 1]->read(j) + path[i + 1]->read(j) - (2.0 * destPoint));
          destPoint += (dataFac + smoothFac);
          change = (destPoint - destPath()[i]->read(j)).abs();
        }
      }
    }
    path = std::move(destPath.path);
  }

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override {
    if (isteps < 1) throw std::runtime_error("SimplePath::generate: isteps is less than 1");

    SimplePath temp;
    if (path.size() > 0) temp().reserve((isteps * (path.size() - 1)) + 1);

    // if path is more than 2 points - interpolation needed
    if (path.size() > 1) {
      // for each segment
      for (size_t i = 0; i < path.size() - 1; i++) {
        // if interpolation needed
        if (isteps > 1) {
          // generate segment
          SimplePath segment = generateSegment(*path[i], *path[i + 1], isteps);
          // move segment into path
          std::move(segment().begin(), segment().end(), std::back_inserter(temp()));
        } else {
          // interpolation not needed
          temp().emplace_back(std::make_shared<Vector>(*path[i]));
        }
      }
    }

    // if path is more than 1 point - return last point
    if (path.size() > 0) temp().emplace_back(path.back());
    return temp;
  }

  /**
   * Convert a DiscretePath to any other type of DiscretePath. It will only convert if T is
   * convertible to C, and it will avoid conversion to itself.
   *
   * @tparam C The type of point for the points to be converted into.
   */
  template <typename C,
            typename = std::enable_if_t<std::is_constructible_v<C, T> && !std::is_same_v<T, C>>>
  operator DiscretePath<C>() const {
    DiscretePath<C> opath;
    opath().reserve(path.size());
    std::transform(path.begin(), path.end(), std::back_inserter(opath()),
                   [](const auto& ipoint) { return std::make_shared<C>(*ipoint); });
    return opath;
  }

  /**
   * Implictly convert path to a shared pointer
   */
  operator std::shared_ptr<AbstractPath>() & override {
    return std::make_shared<DiscretePath<T>>(*this);
  }

  operator std::shared_ptr<AbstractPath>() && override {
    return std::make_shared<DiscretePath<T>>(std::move(*this));
  }

protected:
  /**
   * Sample the segment
   *
   * @param  start  The start point
   * @param  end    The end point
   * @param  isteps the number of points to generate in the segment excluding the end
   * @return the generated segment
   */
  static SimplePath generateSegment(const Vector& start, const Vector& end, const int isteps) {
    if (isteps < 1) throw std::runtime_error("SimplePath::generateSegment: isteps is less than 1");
    SimplePath segment;

    Vector diff = end - start;
    Vector step = diff / isteps;
    // reserve vector capacity
    segment().reserve(isteps);

    for (size_t i = 0; i < isteps; i++) {
      segment().emplace_back(std::make_shared<Vector>(start + (step * i)));
    }

    return segment;
  }

  std::vector<std::shared_ptr<T>> path {};
};

using DataPath = DiscretePath<DataPoint>;
using StatePath = DiscretePath<State>;

} // namespace lib7842