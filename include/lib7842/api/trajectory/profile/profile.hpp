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

struct ProfileFlags {
  Number start_v = 0_pct;
  Number end_v = 0_pct;
  Number top_v = 100_pct;
};

class Profile {
public:
  virtual ~Profile() = default;

  constexpr virtual KinematicState calc(QTime t) const = 0;
  constexpr virtual KinematicState calc(QLength d) const = 0;

  constexpr virtual KinematicState begin() const { return calc(0_s); }
  constexpr virtual KinematicState end() const { return calc(time); }

protected:
  QTime time {0_s}; // the time it takes to complete the profile
};

} // namespace lib7842