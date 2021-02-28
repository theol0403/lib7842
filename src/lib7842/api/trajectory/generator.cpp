#include "lib7842/api/trajectory/generator.hpp"
#include "lib7842/api/other/global.hpp"

namespace lib7842 {

PiecewiseTrapezoidal Generator::generate(const Limits& ilimits, const Limiter& ilimiter,
                                         const Modifier& imodifier, const Executor& iexecutor,
                                         const Spline& spline, const QTime& idt,
                                         const ProfileFlags& flags,
                                         const std::vector<std::pair<Number, Number>>& markers) {
  QLength length = spline.length();
  PiecewiseTrapezoidal profile(ilimits, length, flags, markers);

  // setup
  double t = 0;
  QLength dist = 0_m;
  State pos = spline.calc(t);
  KinematicState k = profile.begin();
  if (k.v == 0_mps) { k = profile.calc(idt); }

  while (dist <= length && t <= 1) {
    // limit the linear velocity according to the abilities of the robot at a spot on the spline
    k.v = std::min(k.v, ilimiter(t));

    // calculate and run motion along trajectory
    iexecutor(imodifier(t, k));

    // calculate distance traveled
    QLength d_dist = k.v * idt;
    dist += d_dist;
    // calculate where along the spline we will be at the end of the timeslice
    t = spline.t_at_dist_travelled(t, d_dist);
    // update new position
    pos = spline.calc(t);
    // calculate new velocity
    k = profile.calc(dist);
  }
  KinematicState end = profile.end();
  if (end.v == 0_mps) { iexecutor(imodifier(1, end)); }
  return profile;
}

// XGenerator::XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
//                        const Limits& ilimits, const ChassisScales& iscales, const QTime& idt) :
//   Generator(ilimits, iscales, idt),
//   model(std::move(imodel)),
//   gearset(igearset),
//   rate(std::shared_ptr<AbstractRate>(global::getTimeUtil()->getRate())) {
//   {}
// }

// void XGenerator::follow(const Spline& spline, bool forward, const ProfileFlags& flags,
//                         const std::vector<std::pair<Number, Number>>& markers) {
//   generate(
//     spline, [this, forward](const Step& s) { run(s, forward); }, flags, markers);
// }

// void XGenerator::run(const Generator::Step& s, bool forward) {
//   QSpeed left = s.k.v / std::sqrt(2) - (s.w / radian * scales.wheelTrack) / 2;
//   QSpeed right = s.k.v / std::sqrt(2) + (s.w / radian * scales.wheelTrack) / 2;

//   QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
//   QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

//   auto leftSpeed = (leftWheel / gearset).convert(number);
//   auto rightSpeed = (rightWheel / gearset).convert(number);

//   if (forward) {
//     model->tank(leftSpeed, rightSpeed);
//   } else {
//     model->tank(-rightSpeed, -leftSpeed);
//   }
//   rate->delayUntil(dt);
// }

// std::tuple<std::vector<Generator::Step>, PiecewiseTrapezoidal>
//   XTestGenerator::follow(const Spline& spline, bool /*forward*/, const ProfileFlags& flags,
//                          const std::vector<std::pair<Number, Number>>& markers) {
//   std::vector<Step> trajectory;
//   auto profile = generate(
//     spline,
//     [&](const Step& s) {
//       QSpeed left = s.k.v / sqrt(2) - (s.w / radian * scales.wheelTrack) / 2;
//       QSpeed right = s.k.v / sqrt(2) + (s.w / radian * scales.wheelTrack) / 2;

//       QAngularSpeed leftWheel = (left / (1_pi * scales.wheelDiameter)) * 360_deg;
//       QAngularSpeed rightWheel = (right / (1_pi * scales.wheelDiameter)) * 360_deg;

//       auto leftSpeed = leftWheel / 200_rpm;
//       auto rightSpeed = rightWheel / 200_rpm;

//       Step ns = s;
//       ns.left = leftSpeed;
//       ns.right = rightSpeed;
//       trajectory.emplace_back(ns);
//     },
//     flags, markers);
//   return {trajectory, profile};
// }

} // namespace lib7842