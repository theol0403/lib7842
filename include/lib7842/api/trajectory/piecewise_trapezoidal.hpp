#pragma once
#include "trapezoidal.hpp"
#include <numeric>

namespace lib7842 {

class PiecewiseTrapezoidal : public Profile {
public:
  PiecewiseTrapezoidal(const Limits& ilimits,
                       const std::vector<std::pair<QLength, ProfileFlags>>& isegments) {
    std::transform(isegments.begin(), isegments.end(), std::back_inserter(segments),
                   [&](const std::pair<QLength, ProfileFlags>& pair) {
                     return Trapezoidal(ilimits, pair.first, pair.second);
                   });

    time = std::accumulate(
      segments.begin(), segments.end(), 0_s,
      [](const QTime& totalT, const Trapezoidal& profile) { return totalT + profile.end().t; });
  }

  KinematicState calc(QTime t) const override {
    QTime totalT = 0_s;
    for (auto&& segment : segments) {
      if (t >= segment.begin().t + totalT && t <= segment.end().t + totalT) {
        return segment.calc(t - totalT);
      }
      totalT += segment.end().t;
    }
    return {};
  }

  KinematicState calc(QLength d) const override {
    QLength totalD = 0_m;
    for (auto&& segment : segments) {
      if (d >= segment.begin().d + totalD && d <= segment.end().d + totalD) {
        return segment.calc(d - totalD);
      }
      totalD += segment.end().d;
    }
    return {};
  }

  KinematicState begin() const override { return segments.front().begin(); }
  KinematicState end() const override { return segments.back().end(); }

protected:
  std::vector<Trapezoidal> segments {};
};
} // namespace lib7842
