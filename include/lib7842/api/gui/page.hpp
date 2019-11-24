#pragma once

#include "okapi/api/util/logging.hpp"
#include <memory>
#include <string>

#ifndef THREADS_STD
  #include "display/lvgl.h"
#else
  #include "lvgl/lvgl.h"
#endif

namespace lib7842::GUI {

using namespace okapi;

/**
 * An LVGL container. This class can be extended to implement pages.
 */
class Page {
public:
  /**
   * Create a new page. Theme color is inherited from the parent.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`
   * @param ilogger The logger
   */
  explicit Page(lv_obj_t* iparent,
                const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  /**
   * Create a new page.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`W
   * @param icolor  The theme color
   * @param ilogger The logger
   */
  explicit Page(lv_obj_t* iparent,
                lv_color_t icolor,
                const std::shared_ptr<Logger>& ilogger = Logger::getDefaultLogger());

  Page(const Page& ipage) = delete;
  Page(Page&& ipage) = default;
  virtual ~Page();

  /**
   * Initialize the page. Override this method to implement custom initialization.
   */
  virtual void initialize() {};

  /**
   * Render the page. Override this method to implement custom rendering.
   */
  virtual void render() {};

  /**
   * Get the lv_obj_t* to the page.
   *
   * @return The page.
   */
  lv_obj_t* getPage() const;

protected:
  lv_obj_t* container;
  lv_style_t cStyle;
  const lv_color_t themeColor;

  std::shared_ptr<Logger> logger {nullptr};
};

} // namespace lib7842::GUI
