#pragma once
#include "abstractPath.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace lib7842 {

template <typename T = Vector> class Path {
public:
  Path() = default;
  virtual ~Path() = default;

  virtual T calc(double t) const = 0;

  virtual double curvature(double t) const = 0;

  virtual double velocity(double t) const {
    return length();
  }

  virtual DiscretePath<T> interpolate(double steps) const {
    std::vector<T> temp;
    temp.reserve(steps + 1);
    for (size_t i = 0; i < steps + 1; i++) {
      temp.emplace_back(calc(i / steps));
    }
    return DiscretePath<T>(std::move(temp));
  }

  virtual QLength length(double resolution = 100) const {
    QLength len {0.0};
    for (size_t i = 0; i < resolution; i++) {
      len += calc(i / resolution).distTo(calc((i + 1) / resolution));
    }
    return len;
  }

  virtual double t_at_dist_travelled(double t, const QLength& dist) const {
    return t + dist / std::abs(velocity(t));
  }
};
} // namespace lib7842