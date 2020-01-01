#pragma once
#include "lib7842/vision/container.hpp"
#include "page.hpp"
#include <map>

namespace lib7842::GUI {

class VisionLayer;

class VisionDrawer : public Page {
public:
  using Page::Page;

  /**
   * Initialize the vision drawer.
   */
  void initialize() override;

  /**
   * Get the default tab name.
   *
   * @return The name.
   */
  static std::string getName();

  VisionDrawer& clear();
  VisionLayer makeLayer();
};

class VisionLayer {
public:
  VisionLayer(lv_obj_t* canvas);

  VisionLayer& withColor(const lv_color_t& color);
  VisionLayer& withColor(const lv_color_t& color, uint16_t sig);

  void draw(const Vision::Container& container);

protected:
  lv_obj_t* canvas {nullptr};
  lv_color_t defaultColor {LV_COLOR_WHITE};
  std::map<uint16_t, lv_color_t> sigColors {};
};

} // namespace lib7842::GUI
