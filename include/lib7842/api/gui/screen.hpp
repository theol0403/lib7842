#pragma once

#include "lib7842/api/other/taskWrapper.hpp"
#include "page.hpp"
#include <string>

namespace lib7842::GUI {

using namespace okapi;

/**
 * An LVGL TabView that manages Pages. It contains its own task for rendering.
 */
class Screen : public Page, public TaskWrapper {
public:
  /**
   * Create a new screen. Theme color is inherited from the parent.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`
   * @param ilogger The logger
   */
  explicit Screen(lv_obj_t* iparent,
                  const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  /**
   * Create a new screen.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`
   * @param icolor  The theme color
   * @param ilogger The logger
   */
  explicit Screen(lv_obj_t* iparent,
                  lv_color_t icolor,
                  const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  Screen(const Screen& iscreen) = delete;
  Screen(Screen&& iscreen) = default;
  virtual ~Screen();

  /**
   * Create a new tab and return the LVGL pointer.
   *
   * @param  iname The tab name
   * @return LVGL pointer to the new tab
   */
  lv_obj_t* newPage(const std::string& iname);

  /**
   * Helper function to create a new page. Example use: `Screen::newPage<Graph>()`. Returns a
   * reference to the newly created page for configuration purposes.
   *
   * @param  iname The name of the page
   * @tparam T     The Page type to create
   * @return reference to the newly created page
   */
  template <typename T> T& makePage(const std::string& iname = T::getName()) {
    static_assert(std::is_base_of<Page, T>::value, "T is not a Page");
    auto ptr = std::make_shared<T>(newPage(iname), themeColor, Page::logger);
    ptr->initialize();
    pages.emplace_back(ptr);
    return *ptr;
  }

  /**
   * Render the page. Override this method to implement custom rendering.
   */
  void render() override;

  /**
   * Task to render the page.
   */
  void loop() override;

private:
  lv_obj_t* tabview {nullptr};
  std::vector<std::shared_ptr<Page>> pages {};

  lv_style_t style_bg;
  lv_style_t style_indic;
  lv_style_t style_btn_bg;
  lv_style_t style_rel;
  lv_style_t style_tgl_rel;
  lv_style_t style_pr;
  lv_style_t pageStyle;
};

} // namespace lib7842::GUI
