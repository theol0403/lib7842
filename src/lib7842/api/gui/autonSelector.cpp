#include "autonSelector.hpp"

namespace lib7842 {

void AutonSelector::initialize() {
  ButtonMatrix::initialize();
  lv_btnm_set_action(btnm, btnAction);
  lv_btnm_set_toggle(btnm, true, 0);

  inaStyle.body.border.width = 0;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, &inaStyle);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, &inaStyle);
}

std::string AutonSelector::getName() {
  return "AutonSelector";
}

void AutonSelector::run() {
  buttons.at(currentIndex).second();
}

lv_res_t AutonSelector::btnAction(lv_obj_t* ibtnm, const char* itxt) {
  AutonSelector& that = *static_cast<AutonSelector*>(lv_obj_get_free_ptr(ibtnm));
  std::string txt(itxt);

  auto it = std::find_if(that.buttons.begin(), that.buttons.end(), [&](auto& button) {
    return button.first == txt;
  });

  that.currentIndex = it - that.buttons.begin();

  return LV_RES_OK;
}

} // namespace lib7842