#pragma once

#include "lib7842/api/other/utility.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "page.hpp"
#include <functional>
#include <string>

namespace lib7842::GUI {

class Odom : public Page {
public:
  using Page::Page;

  /**
   * Initialize the odomDebug.
   */
  void initialize() override;

  /**
   * Render the odomDebug.
   */
  void render() override;

  /**
   * Get the default tab name.
   *
   * @return The name.
   */
  static std::string getName();

  /**
   * Provide the odometry for displaying.
   *
   * @param iodom The odometry
   */
  Odom& attachOdom(const std::shared_ptr<Odometry>& iodom);

  /**
   * Specify the function to be called when the reset button is pressed. The default resetter is
   * only able to reset the state but not the sensors.
   *
   * @param  iresetter a function that resets the odometry and sensors
   */
  Odom& attachResetter(const std::function<void()>& iresetter);

private:
  std::shared_ptr<Odometry> odom {nullptr};
  std::function<void()> resetter {nullptr};

  void initializeField();
  void initializeText();
  void initializeButton();

  void updateOdom();

  // field
  double fieldDim = 0; // width and height of field container
  lv_style_t fStyle;

  // tile styles
  lv_style_t gry;
  lv_style_t red;
  lv_style_t blu;

  // robot point
  lv_obj_t* led {nullptr};
  lv_style_t ledStyle;

  // robot line
  lv_obj_t* line {nullptr};
  std::vector<lv_point_t> linePoints {lv_point_t {0, 0}, lv_point_t {0, 0}}; // line positions
  int lineLength = 0;
  lv_style_t lineStyle;

  // status label
  lv_obj_t* statusLabel {nullptr};
  lv_style_t textStyle;

  // reset button styles
  lv_style_t resetRel;
  lv_style_t resetPr;

  static lv_res_t tileAction(lv_obj_t*); // action when tile is pressed
  static lv_res_t resetAction(lv_obj_t*); // action when reset button is pressed

  bool hasWarnedRender = false;
};

} // namespace lib7842::GUI
