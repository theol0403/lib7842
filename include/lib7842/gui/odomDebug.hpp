#pragma once
#include "main.h"
#include "lib7842/gui/page.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

class OdomDebug : public Page {

public:
  using Page::Page;

  void initialize() override;
  void render() override;

  /**
   * Attach the odom for reading
   *
   * @param iodom The odometry
   */
  void attachOdom(const std::shared_ptr<Odometry>& iodom);

  /**
   * Sets the function to be called when the reset button is pressed
   * The default resetter is only able to reset the state, not the sensors
   * @param callback a function that resets the odometry and sensors
   */
  void attachResetter(const std::function<void()>& iresetter);

protected:
  std::shared_ptr<Odometry> odom {nullptr};
  std::function<void()> resetter {nullptr};

private:
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

} // namespace lib7842
