#pragma once
#include "pathFollower.hpp"

namespace lib7842 {

class UnicycleFollower {
public:
  UnicycleFollower(std::shared_ptr<ChassisModel> imodel, std::shared_ptr<Odometry> iodometry,
                   const QSpeed& imaxVel, const QAngularSpeed& imaxTurnVel,
                   const ChassisScales& ichassisScales, const QAngularSpeed& igearset);

  virtual ~UnicycleFollower() = default;

  virtual void seek(const State& iref, double ih, double ikv, double ika);

  using u_t = std::tuple<QSpeed, QAngularSpeed>;
  u_t clamp(const u_t& u);

  //   /**
  //    * Set the motor mode for the pursuit. Velocity mode is more accurate, but voltage mode is
  //    * smoother. Default is voltage.
  //    *
  //    * @param imode The motor mode
  //    */
  //   virtual void setMotorMode(util::motorMode imode);

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};

  const QSpeed maxVel;
  const QAngularSpeed maxTurnVel;

  const ChassisScales chassisScales;
  const QAngularSpeed gearset;

  //   util::motorMode mode {util::motorMode::voltage};
};
} // namespace lib7842
