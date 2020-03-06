#pragma once
#include "lib7842/api/other/taskWrapper.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/util/logging.hpp"
#include "okapi/api/util/timeUtil.hpp"

namespace lib7842 {

/**
 * Odometry algorithm that directly uses 5225A's tracking algorithm. Uses the cartesian stateMode.
 */
class CustomOdometry : public Odometry, public TaskWrapper {
public:
  /**
   * CustomOdometry.
   *
   * @param imodel         The chassis model for reading sensors.
   * @param ichassisScales The chassis dimensions.
   */
  CustomOdometry(const std::shared_ptr<ChassisModel>& imodel, const ChassisScales& ichassisScales);

  /**
   * Set the drive and turn scales.
   *
   * @param ichassisScales The chassis scales
   */
  void setScales(const ChassisScales& ichassisScales) override;

  /**
   * Do one odometry step.
   */
  void step() override;

  /**
   * Return the current state.
   *
   * @return The state.
   */
  virtual const State& getState() const;

  /**
   * Return the current state.
   *
   * @param imode The mode to return the state in.
   * @return The current state in the given format.
   */
  OdomState getState(const StateMode& imode) const override;

  /**
   * Set a new state.
   *
   * @param istate The state
   */
  virtual void setState(const State& istate);

  /**
   * Set a new state to be the current state.
   *
   * @param istate The new state in the given format.
   * @param imode The mode to treat the input state as.
   */
  void setState(const OdomState& istate, const StateMode& imode) override;

  /**
   * Reset state to {0, 0, 0}
   */
  virtual void resetState();

  /**
   * Reset sensors and state
   */
  virtual void reset();

  /**
   * Get the chassis model.
   *
   * @return The internal ChassisModel.
   */
  std::shared_ptr<ReadOnlyChassisModel> getModel() override;

  /**
   * Get the chassis scales.
   *
   * @return The internal ChassisScales.
   */
  ChassisScales getScales() override;

  /**
   * Odometry calculation loop
   */
  void loop() override;

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  ChassisScales chassisScales;

  State state {};
  std::valarray<std::int32_t> lastTicks {0, 0, 0};
};
} // namespace lib7842
