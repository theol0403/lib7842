#include "path.hpp"
#include <cstddef>
#include <ranges>

namespace lib7842 {

template <size_t N> class Hermite {
public:
  constexpr Hermite(double start, double start_t, double end, double end_t);
  constexpr ~Hermite() = default;

  constexpr double calc(double x) const {
    double sum {0};
    for (size_t i = 0; i < coeffs.size(); ++i) {
      sum += coeffs[i] * std::pow(x, i);
    }
    return sum;
  }

  constexpr double calc_d(double x) const {
    double sum {0};
    for (size_t i = 1; i < coeffs.size(); ++i) {
      sum += coeffs[i] * i * std::pow(x, i - 1);
    }
    return sum;
  }

  constexpr double calc_d2(double x) const {
    double sum {0};
    for (size_t i = 2; i < coeffs.size(); ++i) {
      sum += coeffs[i] * i * (i - 1) * std::pow(x, i - 2);
    }
    return sum;
  }

  static constexpr size_t order = N;

protected:
  std::array<double, N + 1> coeffs {};
};

template <> constexpr Hermite<3>::Hermite(double start, double start_t, double end, double end_t) {
  double u = end - start;

  double a3 = 3.0 * u - 2.0 * start_t - end_t;
  double a4 = -2.0 * u + start_t + end_t;

  coeffs = {start, start_t, a3, a4};
}

template <> constexpr Hermite<5>::Hermite(double start, double start_t, double end, double end_t) {
  double u = end - start - start_t;
  double v = end_t - start_t;

  double a3 = 10.0 * u - 4.0 * v;
  double a4 = -15.0 * u + 7.0 * v;
  double a5 = 6.0 * u - 3.0 * v;

  coeffs = {start, start_t, 0.0, a3, a4, a5};
}

using CubicHermite = Hermite<3>;
using QuinticHermite = Hermite<5>;

} // namespace lib7842