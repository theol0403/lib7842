#pragma once
#include "trapezoidal.hpp"
#include <numeric>
#include <queue>

namespace lib7842 {

class PiecewiseTrapezoidal : public Profile {
public:
  // a list of distance and velocity percentages that mark the piecewise
  using Markers = std::vector<std::pair<Number, Number>>;

  PiecewiseTrapezoidal(const Limits& ilimits, const QLength& ilength,
                       const ProfileFlags& iflags = {}, const Markers& imarkers = {}) {

    auto allMarkers = std::deque<std::pair<Number, Number>>(imarkers.begin(), imarkers.end());
    allMarkers.emplace_front(0_pct, iflags.start_v);
    allMarkers.emplace_back(100_pct, iflags.end_v);

    std::vector<std::pair<QLength, ProfileFlags>> segments;
    for (size_t i = 1; i < allMarkers.size(); ++i) {
      segments.emplace_back(
        allMarkers.at(i).first * ilength - allMarkers.at(i - 1).first * ilength,
        ProfileFlags {allMarkers.at(i - 1).second, allMarkers[i].second, iflags.top_v});
    }

    std::transform(segments.begin(), segments.end(), std::back_inserter(profiles),
                   [&](const std::pair<QLength, ProfileFlags>& pair) {
                     return Trapezoidal(ilimits, pair.first, pair.second);
                   });

    time = std::accumulate(
      profiles.begin(), profiles.end(), 0_s,
      [](const QTime& totalT, const Trapezoidal& profile) { return totalT + profile.end().t; });
  }

  KinematicState calc(QTime t) const override {
    QTime totalT = 0_s;
    for (auto&& profile : profiles) {
      if (t >= profile.begin().t + totalT && t <= profile.end().t + totalT) {
        return profile.calc(t - totalT);
      }
      totalT += profile.end().t;
    }
    return {};
  }

  KinematicState calc(QLength d) const override {
    QLength totalD = 0_m;
    for (auto&& profile : profiles) {
      if (d >= profile.begin().d + totalD && d <= profile.end().d + totalD) {
        return profile.calc(d - totalD);
      }
      totalD += profile.end().d;
    }
    return {};
  }

  KinematicState begin() const override { return profiles.front().begin(); }
  KinematicState end() const override { return profiles.back().end(); }

protected:
  std::vector<Trapezoidal> profiles {};
};
} // namespace lib7842
