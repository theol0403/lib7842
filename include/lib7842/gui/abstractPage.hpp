#pragma once
#include "main.h"
#include "lib7842/other/taskWrapper.hpp"

namespace lib7842 {

class AbstractPage {

public:
  explicit AbstractPage(lv_obj_t* iparent);
  explicit AbstractPage(lv_obj_t* iparent, lv_color_t icolor);

  AbstractPage(const AbstractPage& ipath) = delete;
  virtual ~AbstractPage();

  virtual void initialize() = 0;
  virtual void render() = 0;
  virtual void destroy() = 0;

protected:
  lv_obj_t* container;
  lv_style_t cStyle;
  const lv_color_t themeColor;
};

} // namespace lib7842
