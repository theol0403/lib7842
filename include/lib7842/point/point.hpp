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

  virtual QPoint operator+(const QPoint& rhs) const;
  virtual QPoint operator-(const QPoint& rhs) const;
  virtual QPoint operator*(const double scalar) const;
  virtual QPoint operator/(const double scalar) const;
  virtual bool operator==(const QPoint& rhs) const;
  virtual bool operator!=(const QPoint& rhs) const;

  virtual QLength& operator[](const size_t& iindex);

  static QPoint normalize(const QPoint& lhs);
  static QPoint scalarMult(const QPoint& lhs, const double scalar);
  static QArea dot(const QPoint& lhs, const QPoint& rhs);
  static QLength mag(const QPoint& lhs);
  static QLength dist(const QPoint& lhs, const QPoint& rhs);
};

} // namespace lib7842