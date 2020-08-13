#pragma once
#include <functional>
#include <memory>
#include <string>

#ifndef THREADS_STD
  #include "display/lvgl.h"
#else
  #include "lvgl/lvgl.h"
#endif

namespace lib7842::GUI {

/**
 * An LVGL container. This class can be extended to implement pages.
 */
class Page {
public:
  Page(const Page&) = delete;
  Page operator=(const Page&) = delete;
  Page(Page&& ipage) noexcept;
  Page& operator=(Page&& ipage) noexcept;
  virtual ~Page();

  /**
   * Create a new page. Theme color is inherited from the parent.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`
   */
  explicit Page(lv_obj_t* iparent);

  /**
   * Create a new page.
   *
   * @param iparent The LVGL parent, typically `lv_scr_act()`W
   * @param icolor  The theme color
   */
  explicit Page(lv_obj_t* iparent, lv_color_t icolor);

  /**
   * Initialize the page. Override this method to implement custom initialization.
   */
  virtual void initialize() {}

  /**
   * Render the page. Override this method to implement custom rendering.
   */
  virtual void render() {}

  /**
   * Get the lv_obj_t* to the page.
   *
   * @return The page.
   */
  lv_obj_t* getPage() const;

protected:
  lv_obj_t* container;
  lv_style_t cStyle {};
  lv_color_t themeColor {};
};
} // namespace lib7842::GUI
