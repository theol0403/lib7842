#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/units/QTime.hpp"

namespace lib7842 {
using namespace okapi;

struct KinematicState {
  QTime t {0_s};
  QLength d {0_m};
  QAcceleration a {0_mps2};
  QSpeed v {0_mps};
};

class Profile {
public:
  virtual ~Profile() = default;

  constexpr virtual KinematicState calc(QTime t) const = 0;
  constexpr virtual KinematicState calc(QLength d) const = 0;
};

} // namespace lib7842