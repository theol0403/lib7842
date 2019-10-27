#include "main.h"
#pragma once

#include "lib7842/positioning/point/state.hpp"
#include "lib7842/other/taskWrapper.hpp"
#include "threeEncXDriveModel.hpp"

namespace lib7842 {
class CustomOdometry : public TaskWrapper {
public:
  /**
   * CustomOdometry.
   *
   * @param itimer         The itimer utility
   * @param imodel         The 3EncX chassis model for reading sensors.
   * @param ichassisScales The chassis dimensions.
   */
  CustomOdometry(const std::shared_ptr<ChassisModel>& imodel, const ChassisScales& ichassisScales);

  virtual ~CustomOdometry() = default;

  /**
   * Do one odometry step.
   */
  void step();

  /**
   * Returns the current state.
   */
  const State& getState() const;

  /**
   * Sets a new state to be the current state.
   */
  void setState(const State& istate);

  /**
   * Resets state to {0, 0, 0}
   */
  void resetState();

  /**
   * Resets sensors and state
   */
  void reset();

  /**
   * Odometry calculation loop
   */
  virtual void loop() override;

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  const ChassisScales chassisScales;
  const double chassisWidth;
  const double middleDistance;

  State state;
  std::valarray<std::int32_t> lastTicks {0, 0, 0};
};
} // namespace lib7842
