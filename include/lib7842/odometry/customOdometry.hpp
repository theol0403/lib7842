#include "main.h"
#pragma once

#include "lib7842/positioning/point/state.hpp"
#include "lib7842/other/taskWrapper.hpp"
#include "threeEncXDriveModel.hpp"

namespace lib7842 {
class CustomOdometry : public TaskWrapper, public Odometry {
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
   * Sets the drive and turn scales.
   */
  virtual void setScales(const ChassisScales& ichassisScales) override;

  /**
   * Do one odometry step.
   */
  virtual void step() override;

  /**
   * Returns the current state.
   */
  virtual const State& getState() const;

  /**
   * Sets a new state to be the current state.
   */
  virtual void setState(const State& istate);

  /**
   * Resets state to {0, 0, 0}
   */
  virtual void resetState();

  /**
   * Resets sensors and state
   */
  virtual void reset();

  /**
   * Odometry calculation loop
   */
  virtual void loop() override;

private:
  virtual OdomState getState(const StateMode& imode) const override;
  virtual void setState(const OdomState& istate, const StateMode& imode) override;

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  ChassisScales chassisScales;
  double chassisWidth;
  double middleDistance;

  State state;
  std::valarray<std::int32_t> lastTicks {0, 0, 0};
};
} // namespace lib7842
