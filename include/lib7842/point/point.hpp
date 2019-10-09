#pragma once
#include "main.h"

namespace lib7842 {

class QPoint {
 public:
  QLength x {0_in};
  QLength y {0_in};

  QPoint() = default;
  QPoint(const QPoint& ipoint) = default;
  virtual ~QPoint() = default;

  QPoint(const QLength& ix, const QLength& iy);

  QPoint operator+(const QPoint& rhs) const;
  QPoint operator-(const QPoint& rhs) const;
  bool operator==(const QPoint& rhs) const;
  bool operator!=(const QPoint& rhs) const;

  QLength& operator[](const size_t& iindex);

  QPoint operator*(const double scalar) const;
  QPoint operator/(const double scalar) const;

  static QPoint normalize(const QPoint& lhs);
  static QPoint scalarMult(const QPoint& lhs, const double scalar);
  static QArea dot(const QPoint& lhs, const QPoint& rhs);
  static QLength mag(const QPoint& lhs);
  static QLength dist(const QPoint& lhs, const QPoint& rhs);
};

using PointReference = std::reference_wrapper<const QPoint>;

} // namespace lib7842