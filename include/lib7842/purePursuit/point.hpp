#pragma once
#include "main.h"

namespace lib7842 {

struct dPoint; //Forward declare

struct QPoint {
  QLength x {0_in};
  QLength y {0_in};
  QAngle theta {0_rad};

  QPoint(const QPoint&);

  QPoint(QLength, QLength, QAngle);
  QPoint(QLength, QLength);
  QPoint();

  QPoint(dPoint);

  QPoint operator+(QPoint);
};

struct dPoint {
  double x {0};
  double y {0};
  double theta {0};

  dPoint(const dPoint&);

  dPoint(double, double, double);
  dPoint(double, double);
  dPoint();

  dPoint(QPoint);

  dPoint operator+(dPoint);
};

} // namespace lib7842