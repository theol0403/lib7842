#include "path.hpp"
#include <cstddef>
#include <ranges>

namespace lib7842 {

template <size_t N> class Bezier {
public:
  constexpr explicit Bezier(const std::array<double, N + 1>& ictrls) : ctrls(ictrls) {}
  constexpr ~Bezier() = default;

  constexpr double calc(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < ctrls.size(); ++i) {
      sum += basis(N, i, x) * ctrls[i];
    }
    return sum;
  }

  constexpr double calc_d(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < N; ++i) {
      sum += basis(N - 1, i, x) * N * (ctrls[i + 1] - ctrls[i]);
    }
    return sum;
  }

  constexpr double calc_d2(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < N - 1; ++i) {
      sum += basis(N - 2, i, x) * N * (N - 1) * (ctrls[i + 2] - 2 * ctrls[i + 1] + ctrls[i]);
    }

    return sum;
  }

protected:
  std::array<double, N + 1> ctrls {};

  static constexpr double basis(size_t n, size_t k, double x) {
    return comb(n, k) * std::pow(1.0 - x, n - k) * std::pow(x, k);
  }

  static constexpr size_t comb(size_t n, size_t k) {
    if (k < 0 or k > n) { return 0; }
    if (k > n - k) { k = n - k; }
    if (k == 0 or n <= 1) { return 1; }
    return factorial(n) / (factorial(k) * factorial(n - k));
  }

  static constexpr size_t factorial(size_t n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
  }
};

template <size_t N> Bezier(const std::array<double, N>&) -> Bezier<N - 1>;
} // namespace lib7842