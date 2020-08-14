#include "bezier.hpp"
#include "hermite.hpp"
#include "path.hpp"
#include <bits/iterator_concepts.h>
#include <cstddef>

namespace lib7842 {

template <typename T> concept ParametricFunction = requires(T t) {
  { t.calc(0.0) }
  ->std::convertible_to<double>;
};

template <ParametricFunction T> class Parametric : public Path {
public:
  constexpr Parametric(T&& ix, T&& iy) : x(std::forward<T>(ix)), y(std::forward<T>(iy)) {}

  constexpr Parametric(const State& start, const State& end) :
    x(T(start.x.convert(meter), cos(start.theta).convert(number), end.x.convert(meter),
        cos(end.theta).convert(number))),
    y(T(start.y.convert(meter), sin(start.theta).convert(number), end.y.convert(meter),
        sin(end.theta).convert(number))) {}

  template <size_t N>
  constexpr explicit Parametric(const std::array<Vector, N>& ictrls) :
    x(Bezier(process(ictrls, [](const Vector& p) { return p.x.convert(meter); }))),
    y(Bezier(process(ictrls, [](const Vector& p) { return p.y.convert(meter); }))) {}

  template <size_t N> constexpr auto process(const std::array<Vector, N>& ictrls, auto&& f) {
    std::array<double, N> t;
    std::transform(std::begin(ictrls), std::end(ictrls), std::begin(t), f);
    return t;
  }

  constexpr State calc(double t) const override {
    QLength x_t = x.calc(t) * meter;
    QLength y_t = y.calc(t) * meter;

    QLength x1_t = x.calc_d(t) * meter;
    QLength y1_t = y.calc_d(t) * meter;

    return State(x_t, y_t, atan2(y1_t, x1_t));
  }

  constexpr QCurvature curvature(double t) const override {
    QLength x_d = x.calc_d(t) * meter;
    QLength y_d = y.calc_d(t) * meter;
    QLength x_d_2 = x.calc_d2(t) * meter;
    QLength y_d_2 = y.calc_d2(t) * meter;
    return ((x_d * y_d_2 - y_d * x_d_2) / pow<3>(sqrt(x_d * x_d + y_d * y_d)));
  }

  constexpr QLength velocity(double t) const override {
    return sqrt(square(x.calc_d(t) * meter) + square(y.calc_d(t) * meter));
  }

protected:
  T x;
  T y;
};

template <ParametricFunction T> Parametric(T&&, T&&) -> Parametric<T>;
template <size_t N> Parametric(const std::array<Vector, N>&) -> Parametric<Bezier<N - 1>>;

}; // namespace lib7842