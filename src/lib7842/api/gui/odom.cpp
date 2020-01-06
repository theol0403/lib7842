#include "odom.hpp"
#include "lib7842/api/other/units.hpp"

namespace lib7842::GUI {

using namespace lib7842::units;

void Odom::initialize() {
  initializeField();
  initializeText();
  initializeButton();
}

void Odom::render() {
  if (odom) {
    updateOdom();
  } else {
    if (!hasWarnedRender) {
      hasWarnedRender = true;
      LOG_WARN_S("Odom::render: odom not attached");
    }
  }
}

Odom& Odom::attachOdom(const std::shared_ptr<Odometry>& iodom) {
  odom = iodom;
  return *this;
}

Odom& Odom::attachResetter(const std::function<void()>& iresetter) {
  resetter = iresetter;
  return *this;
}

void Odom::initializeField() {
  /**
  * Field
  */
  lv_obj_t* field = lv_obj_create(container, NULL);
  fieldDim = std::min(lv_obj_get_width(container), lv_obj_get_height(container));
  lv_obj_set_size(field, fieldDim, fieldDim);
  lv_obj_align(field, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

  /**
   * Field Style
   */
  lv_style_copy(&fStyle, &cStyle);
  fStyle.body.main_color = LV_COLOR_WHITE;
  fStyle.body.grad_color = LV_COLOR_WHITE;
  lv_obj_set_style(field, &fStyle);

  /**
   * Tile Styles
   */
  lv_style_copy(&gry, &lv_style_plain);
  gry.body.main_color = LV_COLOR_HEX(0x828F8F);
  gry.body.grad_color = LV_COLOR_HEX(0x828F8F);
  gry.body.border.width = 1;
  gry.body.radius = 0;
  gry.body.border.color = LV_COLOR_WHITE;

  lv_style_copy(&red, &gry);
  red.body.main_color = LV_COLOR_HEX(0xD42630);
  red.body.grad_color = LV_COLOR_HEX(0xD42630);
  lv_style_copy(&blu, &gry);
  blu.body.main_color = LV_COLOR_HEX(0x0077C9);
  blu.body.grad_color = LV_COLOR_HEX(0x0077C9);

  /**
   * Tile Layout
   */
  lv_style_t* tileData[6][6] = {{&gry, &red, &gry, &gry, &blu, &gry}, //
                                {&red, &gry, &gry, &gry, &gry, &blu}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}, //
                                {&gry, &gry, &gry, &gry, &gry, &gry}};

  double tileDim = fieldDim / 6; // tile dimention

  /**
   * Create tile matrix, register callbacks, assign each tile an ID
   */
  for (size_t y = 0; y < 6; y++) {
    for (size_t x = 0; x < 6; x++) {
      lv_obj_t* tileObj = lv_btn_create(field, NULL);
      lv_obj_set_pos(tileObj, x * tileDim, y * tileDim);
      lv_obj_set_size(tileObj, tileDim, tileDim);
      lv_btn_set_action(tileObj, LV_BTN_ACTION_CLICK, tileAction);
      lv_obj_set_free_num(tileObj, y * 6 + x);
      lv_obj_set_free_ptr(tileObj, this);
      lv_btn_set_toggle(tileObj, false);
      lv_btn_set_style(tileObj, LV_BTN_STYLE_PR, tileData[y][x]);
      lv_btn_set_style(tileObj, LV_BTN_STYLE_REL, tileData[y][x]);
    }
  }

  /**
   * Robot point using lvgl led
   */
  led = lv_led_create(field, NULL);
  lv_led_on(led);
  lv_obj_set_size(led, fieldDim / 15.0, fieldDim / 15.0);

  lv_style_copy(&ledStyle, &lv_style_plain);
  ledStyle.body.radius = LV_RADIUS_CIRCLE;
  ledStyle.body.main_color = themeColor;
  ledStyle.body.grad_color = themeColor;
  ledStyle.body.border.color = LV_COLOR_WHITE;
  ledStyle.body.border.width = 2;
  ledStyle.body.border.opa = LV_OPA_100;
  lv_obj_set_style(led, &ledStyle);

  /**
   * Robot line
   */
  line = lv_line_create(field, NULL);
  lv_line_set_points(line, linePoints.data(), linePoints.size());
  lv_obj_set_pos(line, 0, 0);

  lineLength = fieldDim / 6;

  lv_style_copy(&lineStyle, &lv_style_plain);
  lineStyle.line.width = 3;
  lineStyle.line.opa = LV_OPA_100;
  lineStyle.line.color = themeColor;
  lv_obj_set_style(line, &lineStyle);
}

void Odom::initializeText() {
  statusLabel = lv_label_create(container, NULL);

  lv_style_copy(&textStyle, &lv_style_plain);
  textStyle.text.color = LV_COLOR_WHITE;
  textStyle.text.opa = LV_OPA_100;
  lv_obj_set_style(statusLabel, &textStyle);

  lv_label_set_text(statusLabel, "No odom provided");

  lv_obj_align(statusLabel, container, LV_ALIGN_CENTER,
               -lv_obj_get_width(container) / 2.0 + (lv_obj_get_width(container) - fieldDim) / 2.0,
               0);
}

void Odom::initializeButton() {
  /**
  * Button
  */
  lv_obj_t* btn = lv_btn_create(container, NULL);
  lv_obj_set_size(btn, 100, 40);
  lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_MID,
               -lv_obj_get_width(container) / 2.0 + (lv_obj_get_width(container) - fieldDim) / 2.0,
               0);
  lv_obj_set_free_ptr(btn, this);
  lv_btn_set_action(btn, LV_BTN_ACTION_PR, resetAction);

  /**
   * Style
   */
  lv_style_copy(&resetRel, &lv_style_btn_tgl_rel);
  resetRel.body.main_color = themeColor;
  resetRel.body.grad_color = themeColor;
  resetRel.body.border.color = LV_COLOR_WHITE;
  resetRel.body.border.width = 2;
  resetRel.body.border.opa = LV_OPA_100;
  resetRel.body.radius = 2;
  resetRel.text.color = LV_COLOR_WHITE;

  lv_style_copy(&resetPr, &resetRel);
  resetPr.body.main_color = LV_COLOR_WHITE;
  resetPr.body.grad_color = LV_COLOR_WHITE;
  resetPr.text.color = themeColor;

  lv_btn_set_style(btn, LV_BTN_STYLE_REL, &resetRel);
  lv_btn_set_style(btn, LV_BTN_STYLE_PR, &resetPr);

  /**
   * Reset Button Label
   */
  lv_obj_t* label = lv_label_create(btn, NULL);
  lv_obj_set_style(label, &textStyle);
  lv_label_set_text(label, "Reset");
}

void Odom::updateOdom() {
  OdomState state = odom->getState(StateMode::CARTESIAN);

  // position in court units
  double c_x = state.x.convert(court);
  double c_y = (1_court - state.y).convert(court);
  double c_theta = state.theta.convert(radian);

  // place point on field
  lv_obj_set_pos(led, (c_x * fieldDim) - lv_obj_get_width(led) / 2.0,
                 (c_y * fieldDim) - lv_obj_get_height(led) / 2.0 - 1.0);

  // move start and end of line
  linePoints.at(0) = {(int16_t)((c_x * fieldDim)), (int16_t)((c_y * fieldDim) - (3.0 / 2.0))};
  double newY = lineLength * cos(c_theta);
  double newX = lineLength * sin(c_theta);
  linePoints.at(1) = {(int16_t)(newX + linePoints.at(0).x), (int16_t)(-newY + linePoints.at(0).y)};

  lv_line_set_points(line, linePoints.data(), linePoints.size());
  lv_obj_invalidate(line);

  auto sensors = odom->getModel()->getSensorVals();

  std::string text = "X_in: " + std::to_string(state.x.convert(foot)) + "\n" +
                     "Y_in: " + std::to_string(state.y.convert(foot)) + "\n" +
                     "Theta_deg: " + std::to_string(state.theta.convert(degree)) + "\n" +
                     "Left: " + std::to_string(sensors[0]) + "\n" +
                     "Right: " + std::to_string(sensors[1]);

  if (sensors.size() > 2) text = text + "\n" + "Middle: " + std::to_string(sensors[2]);

  lv_label_set_text(statusLabel, text.c_str());

  lv_obj_align(statusLabel, container, LV_ALIGN_CENTER,
               -lv_obj_get_width(container) / 2.0 + (lv_obj_get_width(container) - fieldDim) / 2.0,
               0);
}

/**
 * Sets odom state when tile is pressed
 * Decodes tile ID to find position
 */
lv_res_t Odom::tileAction(lv_obj_t* tileObj) {
  Odom* that = static_cast<Odom*>(lv_obj_get_free_ptr(tileObj));
  int num = lv_obj_get_free_num(tileObj);
  int y = num / 6;
  int x = num - y * 6;
  if (that->odom) {
    that->odom->setState({x * tile + 0.5_tile, 1_court - y * tile - 0.5_tile, 0_deg},
                         StateMode::CARTESIAN);
  } else {
    that->LOG_WARN_S("Odom::tileAction: odom not attached");
  }
  return LV_RES_OK;
}

/**
 * Reset Sensors and Position
 */
lv_res_t Odom::resetAction(lv_obj_t* btn) {
  Odom* that = static_cast<Odom*>(lv_obj_get_free_ptr(btn));
  if (that->resetter) {
    that->resetter();
  } else {
    if (that->odom) {
      that->LOG_INFO_S("Odom::resetAction: using default resetter");
      that->odom->setState({0_in, 0_in, 0_deg});
    } else {
      that->LOG_WARN_S("Odom::resetAction: odom not attached");
    }
  }
  return LV_RES_OK;
}

} // namespace lib7842::GUI