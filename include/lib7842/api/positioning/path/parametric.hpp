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
  constexpr Parametric(T&& x, T&& y) : p(std::forward<T>(x), std::forward<T>(y)) {}

  template <typename U = T, std::enable_if_t<std::is_base_of_v<Hermite<U::order>, U>>* = nullptr>
  constexpr Parametric(const State& start, const State& end) :
    p(U(start.x.convert(meter), cos(start.theta).convert(number), end.x.convert(meter),
        cos(end.theta).convert(number)),
      U(start.y.convert(meter), sin(start.theta).convert(number), end.y.convert(meter),
        sin(end.theta).convert(number))) {}

  template <typename U = T, size_t N = U::order,
            std::enable_if_t<std::is_same_v<Bezier<N>, U>>* = nullptr>
  constexpr explicit Parametric(const Vector (&ictrls)[N + 1]) :
    p(Bezier(process(ictrls, [](const auto& ip) { return ip.x.convert(meter); })),
      Bezier(process(ictrls, [](const auto& ip) { return ip.y.convert(meter); }))) {}

  constexpr State calc(double t) const override {
    QLength x_t = p.first.calc(t) * meter;
    QLength y_t = p.second.calc(t) * meter;

    QLength x1_t = p.first.calc_d(t) * meter;
    QLength y1_t = p.second.calc_d(t) * meter;

    return State(x_t, y_t, atan2(y1_t, x1_t));
  }

  constexpr QCurvature curvature(double t) const override {
    QLength x_d = p.first.calc_d(t) * meter;
    QLength y_d = p.second.calc_d(t) * meter;
    QLength x_d_2 = p.first.calc_d2(t) * meter;
    QLength y_d_2 = p.second.calc_d2(t) * meter;
    return ((x_d * y_d_2 - y_d * x_d_2) / pow<3>(sqrt(x_d * x_d + y_d * y_d)));
  }

  constexpr QLength velocity(double t) const override {
    return sqrt(square(p.first.calc_d(t) * meter) + square(p.second.calc_d(t) * meter));
  }

protected:
  std::pair<T, T> p;

private:
  template <size_t N> constexpr auto process(const Vector (&ictrls)[N], auto&& f) {
    std::array<double, N> t;
    std::transform(std::begin(ictrls), std::end(ictrls), std::begin(t), f);
    return t;
  }
};

template <ParametricFunction T> Parametric(T&&, T&&) -> Parametric<T>;
template <size_t N> Parametric(const Vector (&)[N]) -> Parametric<Bezier<N - 1>>;

}; // namespace lib7842