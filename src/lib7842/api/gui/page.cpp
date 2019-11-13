#include "page.hpp"

namespace lib7842 {

Page::Page(lv_obj_t* iparent, const std::shared_ptr<Logger>& ilogger) :
  Page(iparent, lv_obj_get_style(iparent)->body.main_color, ilogger) {}

Page::Page(lv_obj_t* iparent, lv_color_t icolor, const std::shared_ptr<Logger>& ilogger) :
  container(lv_obj_create(iparent, NULL)), themeColor(icolor), logger(ilogger) {
  lv_obj_set_size(container, lv_obj_get_width(iparent), lv_obj_get_height(iparent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_copy(&cStyle, &lv_style_plain_color);
  cStyle.body.main_color = themeColor;
  cStyle.body.grad_color = themeColor;
  lv_obj_set_style(container, &cStyle);
}

Page::~Page() {
  lv_obj_del(container);
}

lv_obj_t* Page::getPage() const {
  return container;
}

} // namespace lib7842