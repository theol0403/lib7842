#pragma once
#include "main.h"
#include "lib7842/gui/abstractPage.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

class OdomDebug : public AbstractPage {

public:
  using AbstractPage::AbstractPage;

  void initialize() override;
  void render() override;
  void destroy() override;

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
  void useResetter(const std::function<void()>& iresetter);

protected:
  std::shared_ptr<Odometry> odom {nullptr};
  std::function<void()> resetter {nullptr};

private:
  // field
  lv_style_t fStyle;
  double fieldDim = 0; // width and height of field container

  // tile styles
  lv_style_t gry;
  lv_style_t red;
  lv_style_t blu;

  // robot point
  lv_obj_t* led {nullptr};
  lv_style_t ledStyle;

  // robot line
  lv_obj_t* line {nullptr};
  lv_style_t lineStyle;
  std::vector<lv_point_t> linePoints {{0, 0}, {0, 0}}; // line positions
  int lineWidth = 0;
  int lineLength = 0;

  // status label
  lv_obj_t* statusLabel {nullptr};
  lv_style_t textStyle;

  // reset button styles
  lv_style_t resetRel;
  lv_style_t resetPr;

  static lv_res_t tileAction(lv_obj_t*); // action when tile is pressed
  static lv_res_t resetAction(lv_obj_t*); // action when reset button is pressed
};

} // namespace lib7842
