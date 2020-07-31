#include "lib7842/api/gui/visionDrawer.hpp"
#include <algorithm>
#include <iostream>

namespace lib7842::GUI {

using namespace lib7842::Vision;

VisionPage& VisionPage::clear() {
  std::for_each(objects.begin(), ++iterator,
                [](const auto& object) { lv_obj_set_hidden(object.first, true); });
  iterator = objects.before_begin();
  return *this;
}

VisionPage& VisionPage::draw(const Object& object, const lv_color_t& color) {
  return draw(object, color, color);
}

VisionPage& VisionPage::draw(const Object& object, const lv_color_t& main,
                             const lv_color_t& border) {
  auto& [obj, style] = addObject();
  style.body.main_color = main;
  style.body.grad_color = main;
  style.body.border.color = border;
  lv_obj_set_style(obj, &style);
  lv_obj_set_hidden(obj, false);

  double wScale = lv_obj_get_width(container) / object.get(Query::fovWidth);
  double hScale = lv_obj_get_height(container) / object.get(Query::fovHeight);

  lv_obj_set_x(obj, object.get(Query::x) * wScale);
  lv_obj_set_y(obj, object.get(Query::y) * hScale);
  lv_obj_set_width(obj, object.get(Query::width) * wScale);
  lv_obj_set_height(obj, object.get(Query::height) * hScale);

  return *this;
}

VisionPage& VisionPage::draw(const Vision::Container& icontainer, const lv_color_t& color) {
  for (auto&& object : icontainer()) {
    draw(object, color);
  }
  return *this;
}

VisionPage& VisionPage::draw(const Vision::Container& icontainer, const lv_color_t& main,
                             const lv_color_t& border) {
  for (auto&& object : icontainer()) {
    draw(object, main, border);
  }
  return *this;
}

VisionLayer VisionPage::makeLayer() {
  return VisionLayer(*this);
}

VisionPage::ScreenObject& VisionPage::addObject() {
  auto attempt = iterator;
  ++attempt;
  if (attempt == objects.end()) {
    auto object = std::make_pair(lv_obj_create(container, NULL), lv_style_t());
    auto& [obj, style] = object;
    lv_obj_set_hidden(obj, true);
    lv_style_copy(&style, &lv_style_pretty_color);
    style.body.border.width = 2;
    style.body.border.opa = LV_OPA_100;
    objects.emplace_after(iterator, std::move(object));
  }
  return *(++iterator);
}

VisionLayer::VisionLayer(VisionPage& idrawer) : drawer(idrawer) {}

VisionLayer& VisionLayer::withColor(const lv_color_t& color) {
  defaultColor = std::make_pair(color, color);
  return *this;
}

VisionLayer& VisionLayer::withColor(const lv_color_t& main, const lv_color_t& border) {
  defaultColor = std::make_pair(main, border);
  return *this;
}

VisionLayer& VisionLayer::withColor(const lv_color_t& color, uint16_t sig) {
  sigColors[sig] = std::make_pair(color, color);
  return *this;
}

VisionLayer& VisionLayer::withColor(const lv_color_t& main, const lv_color_t& border,
                                    uint16_t sig) {
  sigColors[sig] = std::make_pair(main, border);
  return *this;
}

void VisionLayer::draw(const Vision::Container& container) {
  for (auto&& object : container()) {
    auto it = sigColors.find(object.get(Query::sig));
    Style& color = it == sigColors.end() ? defaultColor : it->second;
    drawer.draw(object, color.first, color.second);
  }
}

} // namespace lib7842::GUI
