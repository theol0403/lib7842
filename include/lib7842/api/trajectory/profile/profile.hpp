#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/units/QTime.hpp"

namespace lib7842 {
using namespace okapi;

template <class Unit = QLength> class Profile {
public:
  using Speed = decltype(Unit {1.0} / QTime {1.0});
  using Accel = decltype(Speed {1.0} / QTime {1.0});

  struct State {
    QTime t {0.0};
    Unit d {0.0};
    Accel a {0.0};
    Speed v {0.0};

    Unit length {0.0}; // destination length
    Speed vel {0.0}; // top achivable velocity
    QTime time {0.0}; // destination time
  };

  struct Flags {
    Number start_v = 0_pct;
    Number end_v = 0_pct;
    Number top_v = 100_pct;
  };

  virtual ~Profile() = default;

  constexpr virtual State calc(QTime t) const = 0;
  constexpr virtual State calc(Unit d) const = 0;

  constexpr virtual State begin() const { return calc(0_s); }
  constexpr virtual State end() const { return calc(time); }

protected:
  QTime time {0_s}; // the time it takes to complete the profile
};

} // namespace lib7842
