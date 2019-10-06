#pragma once
#include "main.h"

namespace lib7842 {

class QPoint {
 public:
  QLength x {0_in};
  QLength y {0_in};

  QPoint(const QLength& ix, const QLength& iy);
  QPoint() = default;
  QPoint(const QPoint& ipoint) = default;
  virtual ~QPoint() = default;

  virtual QPoint operator+(const QPoint& rhs) const;
  virtual QPoint operator-(const QPoint& rhs) const;
  virtual bool operator==(const QPoint& rhs) const;
  virtual bool operator!=(const QPoint& rhs) const;

  virtual QLength& operator[](const size_t& iindex) final;

  QPoint normalize() const;
  QPoint scalarMult(const double& scalar) const;
  QArea dot(const QPoint& rhs) const;
  QLength mag() const;
  QLength dist(const QPoint& rhs) const;
};

} // namespace lib7842