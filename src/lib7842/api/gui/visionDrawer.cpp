#include "visionDrawer.hpp"

namespace lib7842::GUI {

void VisionDrawer::initialize() {
  canvas = lv_canvas_create(container, NULL);

  auto width = lv_obj_get_width(container);
  auto height = lv_obj_get_height(container);
  buffer.resize((lv_img_color_format_get_px_size(LV_IMG_CF_TRUE_COLOR_ALPHA) * width * height) / 8);
  lv_canvas_set_buffer(canvas, buffer.data(), width, height, LV_IMG_CF_TRUE_COLOR_ALPHA);

  clear();
}

std::string VisionDrawer::getName() {
  return "Vision";
}

VisionDrawer& VisionDrawer::clear() {
  lv_canvas_flood_fill(canvas, 0, 0, themeColor, themeColor);
  return *this;
}

VisionLayer VisionDrawer::makeLayer() {
  return VisionLayer(canvas);
}

VisionLayer::VisionLayer(lv_obj_t* icanvas) : canvas(icanvas) {}

VisionLayer& VisionLayer::withColor(const lv_color_t& color) {
  defaultColor = color;
  return *this;
}

VisionLayer& VisionLayer::withColor(const lv_color_t& color, uint16_t sig) {
  sigColors[sig] = color;
  return *this;
}

void VisionLayer::draw(const Vision::Container& container) {}

} // namespace lib7842::GUI