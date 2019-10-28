#include "abstractPage.hpp"

namespace lib7842 {

AbstractPage::AbstractPage(lv_obj_t* iparent) :
  AbstractPage(iparent, lv_obj_get_style(iparent)->body.main_color) {}

AbstractPage::AbstractPage(lv_obj_t* iparent, lv_color_t icolor) : parent(iparent), color(icolor) {}

} // namespace lib7842