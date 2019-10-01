#pragma once
#include "main.h"

namespace lib7842 {

struct QState; //forward declare

struct QPoint {
  QLength x {0_in};
  QLength y {0_in};

  QPoint(const QLength& ix, const QLength& iy);
  QPoint() = default;
  explicit QPoint(const QPoint& ipoint) = default;

  virtual QPoint operator+(const QPoint& rhs) const;
  virtual QPoint operator-(const QPoint& rhs) const;
  virtual bool operator==(const QPoint& rhs) const;
  virtual bool operator!=(const QPoint& rhs) const;

  QLength& operator[](const size_t& iindex);

  QPoint normalize() const;
  QPoint scalarMult(const QLength& scalar) const;
  QArea dot(const QPoint& rhs) const;
  QLength mag() const;
  QLength dist(const QPoint& rhs) const;
};

struct QState : public QPoint {
  QAngle theta {0_rad};

  using QPoint::QPoint;
  QState(const QLength& ix, const QLength& iy, const QAngle& itheta);
  explicit QState(const QState& istate) = default;
  explicit QState(const QPoint& ipoint);

  virtual QState operator+(const QState& rhs) const;
  virtual QState operator-(const QState& rhs) const;
  virtual bool operator==(const QState& rhs) const;
  virtual bool operator!=(const QState& rhs) const;
};

} // namespace lib7842