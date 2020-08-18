#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QLength.hpp"
#include "path.hpp"
#include <cstddef>
#include <numeric>

namespace lib7842 {

template <typename P, size_t N> class Piecewise : public PathHelper<Piecewise<P, N>> {
public:
  constexpr Piecewise() = default;
  constexpr explicit Piecewise(P(&&ip)[N]) : p(ip) {}
  constexpr ~Piecewise() override = default;

  constexpr State calc(double t) const override { return get(t, &P::calc); }
  constexpr QCurvature curvature(double t) const override { return get(t, &P::curvature); }
  constexpr QLength velocity(double t) const override { return get(t, &P::velocity) * N; }
  constexpr QLength length(double /*resolution*/) const override {
    return std::accumulate(std::begin(p), std::end(p), 0_m,
                           [](const QLength& l, const P& ip) { return l + ip.length(); });
  }

protected:
  P p[N];

  constexpr auto get(double t, const auto& f) const {
    size_t i = t * N; // which arc to use
    // use t = 1 for the last arc
    if (i == N) { i = N - 1; }
    double x = t * N - i; // which t to use
    return std::invoke(f, p[i], x);
  }
};

template <typename P, size_t N> Piecewise(P(&&)[N]) -> Piecewise<P, N>;

template <typename P, size_t N> constexpr Piecewise<P, N> make_piecewise(P(&&ip)[N]) {
  return Piecewise<P, N>(std::move(ip));
}
} // namespace lib7842