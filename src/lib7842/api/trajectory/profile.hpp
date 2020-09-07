#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/units/QTime.hpp"

namespace lib7842 {
using namespace okapi;

struct Kinematics {
  QTime t;
  QLength d;
  QAcceleration a;
  QSpeed v;
};

class Profile {
public:
  virtual ~Profile() = default;

  constexpr virtual Kinematics calc(const QTime& t) const = 0;
  constexpr virtual Kinematics calc(const QLength& d) const = 0;
};

} // namespace lib7842