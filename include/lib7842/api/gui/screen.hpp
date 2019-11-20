#pragma once

#include "lib7842/api/other/taskWrapper.hpp"
#include "page.hpp"
#include <string>

namespace lib7842 {

using namespace okapi;

class Screen : public TaskWrapper {
public:
  /**
   * Creates a new screen. Theme color is inherited from the parent.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`
   * @param ilogger The logger
   */
  explicit Screen(
    lv_obj_t* iparent, const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  /**
   * Creates a new screen.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`W
   * @param icolor  The theme color
   * @param ilogger The logger
   */
  explicit Screen(
    lv_obj_t* iparent,
    lv_color_t icolor,
    const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  Screen(const Screen& ipage) = delete;
  virtual ~Screen();

  /**
   * Creates a new tab and returns the LVGL pointer.
   *
   * @param  iname The tab name
   * @return LVGL pointer to the new tab
   */
  lv_obj_t* newPage(const std::string& iname);

  /**
   * Helper function to create new page. Example use: `Screen::newPage<Graph>()`. Returns a
   * reference to the newly created page for configuration purposes.
   *
   * @param  iname The name of the page
   * @tparam T     The Page type to create
   * @return reference to the newly created page
   */
  template <typename T> T& makePage(const std::string& iname = T::getName()) {
    static_assert(std::is_base_of<Page, T>::value, "T is not a Page");
    auto ptr = std::make_shared<T>(newPage(iname), themeColor);
    ptr->initialize();
    pages.emplace_back(ptr);
    return *ptr;
  }

  void loop() override;

protected:
  lv_obj_t* tabview {nullptr};
  const lv_color_t themeColor;

  std::shared_ptr<Logger> logger {nullptr};

  std::vector<std::shared_ptr<Page>> pages {};

private:
  lv_style_t style_bg;
  lv_style_t style_indic;
  lv_style_t style_btn_bg;
  lv_style_t style_rel;
  lv_style_t style_tgl_rel;
  lv_style_t style_pr;
  lv_style_t pageStyle;
};

} // namespace lib7842
