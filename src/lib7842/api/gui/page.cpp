#include "lib7842/api/gui/page.hpp"

namespace lib7842::GUI {

Page::Page(Page&& ipage) : container(ipage.container), themeColor(ipage.themeColor) {
  ipage.container = nullptr;
}

Page::Page(lv_obj_t* iparent) : Page(iparent, lv_obj_get_style(iparent)->body.main_color) {}

Page::Page(lv_obj_t* iparent, lv_color_t icolor) :
  container(lv_obj_create(iparent, NULL)), themeColor(icolor) {
  lv_obj_set_size(container, lv_obj_get_width(iparent), lv_obj_get_height(iparent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_copy(&cStyle, &lv_style_plain_color);
  cStyle.body.main_color = themeColor;
  cStyle.body.grad_color = themeColor;
  lv_obj_set_style(container, &cStyle);
}

Page::~Page() {
  if (container) lv_obj_del(container);
}

lv_obj_t* Page::getPage() const {
  return container;
}

} // namespace lib7842::GUI
