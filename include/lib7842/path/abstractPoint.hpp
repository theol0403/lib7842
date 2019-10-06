#pragma once
#include "main.h"

namespace lib7842 {

class AbstractPoint {
 public:
  explicit AbstractPoint() = delete;
  explicit AbstractPoint(const AbstractPoint& ipoint) = delete;
  virtual ~AbstractPoint() = delete;

  virtual const AbstractPoint& operator+(const AbstractPoint& rhs) const = 0;
  virtual const AbstractPoint& operator-(const AbstractPoint& rhs) const = 0;
  virtual bool operator==(const AbstractPoint& rhs) const = 0;
  virtual bool operator!=(const AbstractPoint& rhs) const = 0;

  virtual const QLength& operator[](const size_t& iindex) const = 0;
};

} // namespace lib7842