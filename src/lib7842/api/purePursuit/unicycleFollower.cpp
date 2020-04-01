#include "lib7842/api/purePursuit/unicycleFollower.hpp"

namespace lib7842 {

UnicycleFollower::UnicycleFollower(std::shared_ptr<ChassisModel> imodel,
                                   std::shared_ptr<Odometry> iodometry,
                                   const ChassisScales& ichassisScales,
                                   const QAngularSpeed& igearset) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  chassisScales(ichassisScales),
  gearset(igearset) {}

double sinc(double x) {
  return x == 0 ? 1 : sin(x) / x;
}

double wrap180(double a) {
  return atan2(std::sin(a), std::cos(a));
}

void UnicycleFollower::seek(const State& iref, double ih, double ikv, double ika) {
  // auto rate = global::getTimeUtil()->getRate();

  State pos = State(odometry->getState(StateMode::CARTESIAN));

  double x_ref = iref.y.convert(meter);
  double y_ref = -iref.x.convert(meter);
  double theta_ref = (iref.theta * -1).convert(radian);

  double x = pos.y.convert(meter);
  double y = -pos.x.convert(meter);
  double theta = (pos.theta * -1).convert(radian);

  double ex = cos(theta) * (x_ref - x) - sin(theta) * (y_ref - y);
  double ey = sin(theta) * (x_ref - x) + cos(theta) * (y_ref - y);

  double p = sqrt(ex * ex + ey * ey);

  double phi = atan2(ey, ex) - theta_ref;
  phi = wrap180(phi);
  double a = atan2(ey, ex) - theta;
  a = wrap180(a);

  double v = ikv * p * cos(a);
  double w = ika * a + ikv * sinc(2 * a) * (a + ih * phi);

  auto vel = v * mps;
  auto turnVel = w * radps;

  QAngularSpeed wheelVel = (vel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;
  QAngularSpeed wheelTurnVel = turnVel * chassisScales.wheelTrack / chassisScales.wheelDiameter;

  double wheelPower = (wheelVel / gearset).convert(number);
  double turnPower = (wheelTurnVel / gearset).convert(number);

  auto finalPower = clamp(wheelPower, turnPower);

  double left = finalPower[0] - finalPower[1];
  double right = finalPower[0] + finalPower[1];

  model->tank(left, right);
}

std::valarray<double> UnicycleFollower::clamp(double v, double w) {
  auto sigma = std::max({std::abs(v), std::abs(w), 1.0});

  double v_c;
  double w_c;

  if (sigma == 1.0) {
    v_c = v;
    w_c = w;
  } else if (sigma == v) {
    v_c = util::sgn(v);
    w_c = w / sigma;
  } else {
    v_c = v / sigma;
    w_c = util::sgn(w);
  }

  v_c = std::clamp(v_c, -1.0, 1.0);
  w_c = std::clamp(w_c, -1.0, 1.0);

  return {v_c, w_c};
}

} // namespace lib7842