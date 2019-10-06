#pragma once
#include "main.h"
#include "abstractPoint.hpp"

namespace lib7842 {

struct QPoint : AbstractPoint {
 public:
  QLength x {0_in};
  QLength y {0_in};

  QPoint(const QLength& ix, const QLength& iy);
  QPoint() = default;
  QPoint(const QPoint& ipoint) = default;

  virtual const QPoint& operator+(const QPoint& rhs) const override;
  virtual const QPoint& operator-(const QPoint& rhs) const override;
  virtual bool operator==(const QPoint& rhs) const override;
  virtual bool operator!=(const QPoint& rhs) const override;

  // virtual const QLength& operator[](const size_t& iindex) const override;

  // QPoint normalize() const;
  // QPoint scalarMult(const double& scalar) const;
  // QArea dot(const QPoint& rhs) const;
  // QLength mag() const;
  // QLength dist(const QPoint& rhs) const;
};

} // namespace lib7842