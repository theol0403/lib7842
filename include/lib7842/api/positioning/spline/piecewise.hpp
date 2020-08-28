#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/deps/static_vector.h"
#include "parametric.hpp"
#include "spline.hpp"
#include <numeric>

namespace lib7842 {

template <class P, size_t N> class Piecewise : public SplineHelper<Piecewise<P, N>> {
public:
  constexpr Piecewise() = default;
  constexpr ~Piecewise() override = default;

  constexpr explicit Piecewise(std::static_vector<P, N>&& ip) : p(ip) {}
  constexpr explicit Piecewise(P(&&ip)[N]) : p(std::begin(ip), std::end(ip)) {}

  constexpr State calc(double t) const override { return get(t, &P::calc); }
  constexpr QCurvature curvature(double t) const override { return get(t, &P::curvature); }
  constexpr QLength velocity(double t) const override { return get(t, &P::velocity) * N; }
  constexpr QLength length(double /*resolution*/) const override {
    return std::accumulate(std::begin(p), std::end(p), 0_m,
                           [](const QLength& l, const auto& ip) { return l + ip.length(); });
  }

protected:
  std::static_vector<P, N> p;

  constexpr auto get(double t, const auto& f) const {
    size_t i = t * N; // which arc to use
    // use t = 1 for the last arc
    if (i == N) { i = N - 1; }
    double x = t * N - i; // which t to use
    return std::invoke(f, p[i], x);
  }
};

template <class P, size_t N> Piecewise(P(&&)[N]) -> Piecewise<P, N>;

template <class P, size_t N> constexpr auto make_piecewise(P(&&ip)[N]) {
  return Piecewise<P, N>(std::move(ip));
}

// template <class P, size_t N>
// requires std::same_as<P, Line> constexpr auto make_piecewise(Vector(&&ip)[N]) {
//   // there is one less hermite than points
//   std::array<std::optional<Parametric<P>>, N - 1> p;
//   for (size_t i = 0; i < N - 1; ++i) {
//     p[i].emplace(ip[i], ip[i + 1]);
//   }
//   return Piecewise(std::move(p));
// }
} // namespace lib7842