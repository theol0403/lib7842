#pragma once
#include "arc.hpp"

namespace lib7842 {

/**
 * This class describes a hybrid arc which connects two points together by linearly interpolating
 * between two arcs.
 */
class Mesh : public SplineHelper<Mesh> {
public:
  constexpr Mesh(const State& istart, const State& iend) :
    first(istart, State(iend.x, iend.y, 2 * istart.Vector::angleTo(iend) - istart.theta)),
    second(State(istart.x, istart.y, 2 * istart.Vector::angleTo(iend) - iend.theta), iend) {}

  constexpr State calc(double t) const override {
    auto first_p = first.calc(t);
    auto second_p = second.calc(t);
    return first_p * (1 - t) + second_p * t;
  }

  constexpr QLength velocity(double t) const override {
    auto d = calc_d(t);
    return sqrt(square(d.x) + square(d.y));
  }

  constexpr QCurvature curvature(double t) const override {
    auto d = calc_d(t);
    auto d2 = calc_d2(t);
    return ((d.x * d2.y - d.y * d2.x) / pow<3>(sqrt(d.x * d.x + d.y * d.y)));
  }

  constexpr Vector calc_d(double t) const {
    auto first_p = first.calc(t).vector();
    auto second_p = second.calc(t).vector();
    auto first_d = first.calc_d(t);
    auto second_d = second.calc_d(t);
    return (first_p * -1.0) + first_d * (1 - t) + second_p + second_d * t;
  }

  constexpr Vector calc_d2(double t) const {
    auto first_d = first.calc_d(t);
    auto second_d = second.calc_d(t);
    auto first_d2 = first.calc_d2(t);
    auto second_d2 = second.calc_d2(t);
    return first_d * -2 + first_d2 * (1 - t) + second_d * 2 + second_d2 * t;
  }

protected:
  Arc first;
  Arc second;
};

/**
 * Helper function used to create a Piecewise<Mesh> using an array of states. Connects an arc
 * between each point. The number of arcs in the piecewise is one less than the number of points.
 *
 * @tparam P Must be Mesh.
 * @tparam N The number of points provided.
 * @return A Piecewise<P, N-1>>.
 */
template <class P, size_t N>
requires std::same_as<P, Mesh>
constexpr auto make_piecewise(State(&&ip)[N]) {
  std::array<std::optional<P>, N - 1> p;
  for (size_t i = 0; i < N - 1; ++i) {
    p[i].emplace(ip[i], ip[i + 1]);
  }
  return Piecewise(std::move(p));
}

} // namespace lib7842
