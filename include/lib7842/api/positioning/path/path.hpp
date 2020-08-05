#pragma once
#include "abstractPath.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace lib7842 {

template <typename Prod = Vector> class Path {
public:
  Path() = default;
  virtual ~Path() = default;

  virtual Prod calc(double t) const = 0;

  virtual double curvature(double t) const = 0;

  virtual QLength velocity(double /*t*/) const {
    return length();
  }

  virtual DiscretePath<Prod> interpolate(double steps) const {
    std::vector<Prod> temp;
    temp.reserve(steps + 1);
    for (size_t i = 0; i < steps + 1; i++) {
      temp.emplace_back(calc(i / steps));
    }
    return DiscretePath<Prod>(std::move(temp));
  }

  virtual QLength length(double resolution = 100) const {
    QLength len {0.0};
    for (size_t i = 0; i < resolution; i++) {
      len += calc(i / resolution).distTo(calc((i + 1) / resolution));
    }
    return len;
  }

  virtual double t_at_dist_travelled(double t, const QLength& dist) const {
    return t + (dist / velocity(t).abs()).convert(number);
  }
};
} // namespace lib7842