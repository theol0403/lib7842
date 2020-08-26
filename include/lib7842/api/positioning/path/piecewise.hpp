#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QLength.hpp"
#include "parametric.hpp"
#include "path.hpp"
#include <cstddef>
#include <numeric>

namespace lib7842 {

template <class P, size_t N> class Piecewise : public PathHelper<Piecewise<P, N>> {
public:
  constexpr Piecewise() = default;
  constexpr ~Piecewise() override = default;

  constexpr explicit Piecewise(std::array<std::optional<P>, N>&& ip) : p(ip) {}

  constexpr explicit Piecewise(P(&&ip)[N]) {
    std::move(std::begin(ip), std::end(ip), std::begin(p));
  }

  constexpr State calc(double t) const override { return get(t, &P::calc); }
  constexpr QCurvature curvature(double t) const override { return get(t, &P::curvature); }
  constexpr QLength velocity(double t) const override { return get(t, &P::velocity) * N; }
  constexpr QLength length(double /*resolution*/) const override {
    return std::accumulate(
      std::begin(p), std::end(p), 0_m,
      [](const QLength& l, const std::optional<P>& ip) { return l + ip.value().length(); });
  }

protected:
  std::array<std::optional<P>, N> p {};

  constexpr auto get(double t, const auto& f) const {
    size_t i = t * N; // which arc to use
    // use t = 1 for the last arc
    if (i == N) { i = N - 1; }
    double x = t * N - i; // which t to use
    return std::invoke(f, p[i].value(), x);
  }
};

template <class P, size_t N> Piecewise(P(&&)[N]) -> Piecewise<P, N>;

template <class P, size_t N> constexpr auto make_piecewise(P(&&ip)[N]) {
  return Piecewise<P, N>(std::move(ip));
}

template <class P, size_t N> constexpr auto make_piecewise(State(&&ip)[N]) {
  // there is one less hermite than points
  std::array<std::optional<Parametric<P>>, N - 1> p;
  for (size_t i = 0; i < N - 1; ++i) {
    p[i].emplace(ip[i], ip[i + 1]);
  }

  return Piecewise(std::move(p));
}
} // namespace lib7842