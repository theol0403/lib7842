#pragma once
#include "lib7842/api/vision/container.hpp"
#include "page.hpp"
#include <list>
#include <map>

namespace lib7842::GUI {

class VisionLayer;

class VisionDrawer : public Page {
public:
  using Page::Page;

  VisionDrawer& clear();
  VisionDrawer& draw(const Vision::Object& object, const lv_color_t& color);

  VisionLayer makeLayer();

protected:
  using ScreenObject = std::pair<lv_obj_t*, lv_style_t>;
  ScreenObject& addObject();

  std::list<ScreenObject> objects {};
  size_t objectCount {0};
};

class VisionLayer {
public:
  VisionLayer(VisionDrawer* idrawer);

  VisionLayer& withColor(const lv_color_t& color);
  VisionLayer& withColor(const lv_color_t& color, uint16_t sig);

  void draw(const Vision::Container& container);

protected:
  VisionDrawer* drawer {nullptr};
  lv_color_t defaultColor {LV_COLOR_BLACK};
  std::map<uint16_t, lv_color_t> sigColors {};
};

} // namespace lib7842::GUI
