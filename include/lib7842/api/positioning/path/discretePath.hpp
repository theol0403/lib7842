#pragma once
#include "abstractPath.hpp"

#include "lib7842/api/positioning/point/dataPoint.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <algorithm>
#include <initializer_list>
#include <vector>

namespace lib7842 {

// forward declaration
template <typename T> class DiscretePath;
using SimplePath = DiscretePath<Vector>;
using DataPath = DiscretePath<DataPoint>;
using StatePath = DiscretePath<State>;

/**
 * A path that represents a collection of discrete points which are stored using shared pointers. To
 * have access to the underlying array, use `operator()`.
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
   * The underlying array type
   */
  using array_t = std::vector<std::shared_ptr<T>>;

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
  explicit DiscretePath(const array_t& ipath) : path(ipath) {}

  /**
   * Construct a DiscretePath from any other type of DiscretePath. It will only convert if C is
   * convertible to T, and it will avoid conversion to itself.
   *
   * @param  ipath     The path
   * @tparam C         the type of point to be converted from
   */
  template <typename C,
            typename = std::enable_if_t<std::is_constructible_v<T, C> && !std::is_same_v<T, C>>>
  explicit DiscretePath(const DiscretePath<C>& ipath) {
    path.reserve(ipath().size());
    std::transform(ipath().begin(), ipath().end(), std::back_inserter(path),
                   [](const auto& ipoint) { return std::make_shared<T>(*ipoint); });
  }

  /**
   * Get the underlying array.
   */
  array_t& get() {
    return path;
  }

  const array_t& get() const {
    return path;
  }

  /**
   * Get the underlying array using the () operator.
   */
  array_t& operator()() {
    return path;
  }

  const array_t& operator()() const {
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
  DiscretePath<T>& smoothen(double iweight, const QLength& itolerance) {
    auto temp = copy();
    double smoothWeight = 1.0 - iweight;
    auto change = itolerance;
    while (change >= itolerance) {
      change = 0.0_m;
      for (size_t i = 1; i < path.size() - 1; i++) {
        for (size_t j = 0; j < 2; j++) {
          auto aux = temp()[i]->at(j);
          auto dataFac = iweight * (path[i]->at(j) - aux);
          auto smoothFac =
            smoothWeight * (temp()[i - 1]->at(j) + temp()[i + 1]->at(j) - (2.0 * aux));
          temp()[i]->at(j) += (dataFac + smoothFac);
          change = (aux - temp()[i]->at(j)).abs();
        }
      }
    }
    path = std::move(temp());
    return *this;
  }

  /**
   * Interpolate the path using distance sampling.
   *
   * @param  iresolution The distance between each point
   * @return The generated path
   */
  SimplePath generate(const QLength& iresolution) const {
    SimplePath temp;

    for (size_t i = 0; i < path.size() - 1; i++) {
      Vector& start = *path[i];
      Vector& end = *path[i + 1];

      Vector diff = end - start;
      size_t steps = std::ceil(MathPoint::mag(diff) / iresolution.convert(meter));
      Vector step = diff / steps;

      temp().reserve(temp().size() + steps);
      for (size_t j = 0; j < steps; j++) {
        temp().emplace_back(std::make_shared<Vector>(start + (step * j)));
      }
    }

    // if path is more than 1 point - return last point
    if (path.size() > 0) temp().emplace_back(std::make_shared<Vector>(*path.back()));

    return temp;
  }

  /**
   * Interpolate the path
   *
   * @param  isteps The number of points to interpolate per segment. In the case of DiscretePath, a
   *                segment is defined as the section between two control points, starting from the
   *                first point.
   * @return The generated path
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
          Vector& start = *path[i];
          Vector& end = *path[i + 1];

          Vector diff = end - start;
          Vector step = diff / isteps;

          temp().reserve(temp().size() + isteps);
          for (size_t j = 0; j < isteps; j++) {
            temp().emplace_back(std::make_shared<Vector>(start + (step * j)));
          }
        } else {
          // interpolation not needed
          temp().emplace_back(std::make_shared<Vector>(*path[i]));
        }
      }
    }

    // if path is more than 1 point - return last point
    if (path.size() > 0) temp().emplace_back(std::make_shared<Vector>(*path.back()));
    return temp;
  }

  /**
   * Implicitly convert path to a shared pointer
   */
  operator std::shared_ptr<AbstractPath>() & override {
    return std::make_shared<DiscretePath<T>>(*this);
  }

  operator std::shared_ptr<AbstractPath>() && override {
    return std::make_shared<DiscretePath<T>>(std::move(*this));
  }

protected:
  array_t path {};
};
} // namespace lib7842