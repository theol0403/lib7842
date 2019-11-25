#pragma once
#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"

namespace lib7842 {

class PathFollower {
public:
  PathFollower(const std::shared_ptr<ChassisModel>& imodel,
               const std::shared_ptr<Odometry>& iodometry);

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};
};
} // namespace lib7842
