#pragma once

#include "lib7842/api/other/taskWrapper.hpp"
#include "page.hpp"
#include <string>

namespace lib7842 {

using namespace okapi;

class Screen : public TaskWrapper {

public:
  explicit Screen(
    lv_obj_t* iparent, const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());
  explicit Screen(
    lv_obj_t* iparent,
    lv_color_t icolor,
    const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  Screen(const Screen& ipage) = delete;
  virtual ~Screen();

  /**
   * Creates a new tab
   *
   * @param iname The tab name
   *
   * @return LVGL pointer to the new tab
   */
  lv_obj_t* newPage(const std::string& iname);

  /**
   * Creates a new page
   *
   * @tparam T The page type to create
   *
   * @return Pointer to the newly created page
   */
  template <typename T> T& makePage() {
    static_assert(std::is_base_of<Page, T>::value, "T is not a Page");
    auto ptr = std::make_shared<T>(newPage(T::getName()), themeColor);
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
