#include "buttonMatrix.hpp"

namespace lib7842 {

void ButtonMatrix::initialize() {

  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_obj_set_size(btnm, lv_obj_get_width(container), lv_obj_get_height(container));
  lv_btnm_set_action(btnm, btnAction);
  lv_obj_set_free_ptr(btnm, this);

  /*Create a new style for the button matrix back ground*/
  lv_style_copy(&bgStyle, &lv_style_plain);
  bgStyle.body.main_color = themeColor;
  bgStyle.body.grad_color = themeColor;
  bgStyle.body.border.color = LV_COLOR_WHITE;
  bgStyle.body.border.width = 3;
  bgStyle.body.radius = 0;
  bgStyle.body.padding.inner = 0;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, &bgStyle);

  lv_style_copy(&relStyle, &lv_style_btn_tgl_rel);
  relStyle.body.main_color = themeColor;
  relStyle.body.grad_color = themeColor;
  relStyle.body.border.color = LV_COLOR_WHITE;
  relStyle.body.border.width = 3;
  relStyle.body.border.opa = LV_OPA_100;
  relStyle.body.radius = 5;
  relStyle.text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL, &relStyle);

  lv_style_copy(&prStyle, &relStyle);
  prStyle.body.main_color = LV_COLOR_WHITE;
  prStyle.body.grad_color = LV_COLOR_WHITE;
  prStyle.text.color = themeColor;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_PR, &prStyle);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, &prStyle);

  lv_style_copy(&inaStyle, &lv_style_btn_ina);
  inaStyle.body.empty = true;
  inaStyle.body.border.width = 0;
  inaStyle.text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, &inaStyle);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, &inaStyle);
}

std::string ButtonMatrix::getName() {
  return "ButtonMatrix";
}

} // namespace lib7842