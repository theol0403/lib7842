#include "lib7842/api/trajectory/generator/generator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

PiecewiseTrapezoidal Generator::generate(const Limits<>& limits, const Runner& runner,
                                         const Spline& spline, const QTime& dt,
                                         const Profile<>::Flags& flags,
                                         const PiecewiseTrapezoidal::Markers& markers) {
  auto rate = global::getTimeUtil()->getRate();
  QLength length = spline.length();
  PiecewiseTrapezoidal profile(limits, length, flags, markers);

  // setup
  double t = 0;
  QLength dist = 0_m;
  Profile<>::State k = profile.begin();
  if (k.v == 0_mps) { k = profile.calc(dt); }

  while (dist <= length && t <= 1) {
    // calculate and run motion along trajectory
    runner(t, k);

    // calculate distance traveled
    QLength d_dist = k.v * dt;
    dist += d_dist;
    // calculate where along the spline we will be at the end of the timeslice
    t = spline.t_at_dist_travelled(t, d_dist);
    // calculate new velocity
    k = profile.calc(dist);

#ifndef THREADS_STD
    rate->delayUntil(dt);
#endif
  }
  Profile<>::State end = profile.end();
  if (end.v == 0_mps) { runner(1, end); }
  return profile;
}

Number Generator::toWheel(const QSpeed& v, const ChassisScales& scales,
                          const QAngularSpeed& gearset) {
  return (v / (1_pi * scales.wheelDiameter * gearset)) * 360_deg;
}

} // namespace lib7842
