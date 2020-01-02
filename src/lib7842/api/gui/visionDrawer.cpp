#include "visionDrawer.hpp"
#include <algorithm>

namespace lib7842::GUI {

using namespace lib7842::Vision;

VisionDrawer& VisionDrawer::clear() {
  std::for_each(objects.begin(), objects.begin() + objectCount,
                [](const auto& object) { lv_obj_set_hidden(object.first, true); });
  return *this;
}

VisionDrawer& VisionDrawer::draw(const Object& object, const lv_color_t& color) {
  auto& [obj, style] = addObject();
  style.body.main_color = color;
  style.body.grad_color = color;
  lv_obj_refresh_style(obj);
  lv_obj_set_hidden(obj, false);

  double wScale = lv_obj_get_width(container) / object.get(Query::fovWidth);
  double hScale = lv_obj_get_height(container) / object.get(Query::fovHeight);

  lv_obj_set_x(obj, object.get(Query::x) * wScale);
  lv_obj_set_y(obj, object.get(Query::y) * hScale);

  lv_obj_set_width(obj, object.get(Query::width) * wScale);
  lv_obj_set_height(obj, object.get(Query::height) * hScale);

  return *this;
}

VisionLayer VisionDrawer::makeLayer() {
  return VisionLayer(this);
}

VisionDrawer::ScreenObject& VisionDrawer::addObject() {
  if (objectCount + 1 > objects.size()) {
    auto object = std::make_pair(lv_obj_create(container, NULL), lv_style_t());
    auto& [obj, style] = object;
    lv_obj_set_hidden(obj, true);
    style.body.main_color = LV_COLOR_BLACK;
    style.body.grad_color = LV_COLOR_BLACK;
    style.body.border.width = 0;
    lv_obj_set_style(obj, &style);
    objects.emplace_back(std::move(object));
  }
  objectCount++;
  return objects.at(objectCount - 1);
}

VisionLayer::VisionLayer(VisionDrawer* idrawer) : drawer(idrawer) {}

VisionLayer& VisionLayer::withColor(const lv_color_t& color) {
  defaultColor = color;
  return *this;
}

VisionLayer& VisionLayer::withColor(const lv_color_t& color, uint16_t sig) {
  sigColors[sig] = color;
  return *this;
}

void VisionLayer::draw(const Vision::Container& container) {
  for (auto&& object : container()) {
    auto it = sigColors.find(object.get(Query::sig));
    lv_color_t color = it == sigColors.end() ? defaultColor : it->second;
    drawer->draw(object, color);
  }
}

} // namespace lib7842::GUI