#pragma once
#include "lib7842/api/vision/container.hpp"
#include "page.hpp"
#include <forward_list>
#include <map>

namespace lib7842::GUI {

class VisionLayer;

class VisionPage : public Page {
public:
  using Page::Page;

  /**
   * Clear all objects on screen. Call this function before redrawing objects.
   */
  VisionPage& clear();

  /**
   * Draw a single object to the screen. Use a single color.
   *
   * @param  object The object
   * @param  color  The color
   */
  VisionPage& draw(const Vision::Object& object, const lv_color_t& color);

  /**
   * Draw a single object to the screen. Use a border color.
   *
   * @param  object The object
   * @param  main   The main color
   * @param  border The border color
   */
  VisionPage& draw(const Vision::Object& object, const lv_color_t& main, const lv_color_t& border);

  /**
   * Draw a container to the screen. Use a single color.
   *
   * @param  icontainer The container
   * @param  color      The color
   */
  VisionPage& draw(const Vision::Container& icontainer, const lv_color_t& color);

  /**
   * Draw a container to the screen. Use a border color.
   *
   * @param  icontainer The container
   * @param  main       The main color
   * @param  border     The border color
   */
  VisionPage& draw(const Vision::Container& icontainer, const lv_color_t& main,
                   const lv_color_t& border);

  /**
   * Create a layer for the screen. Used to draw different colors for different signatures.
   *
   * @return The vision layer.
   */
  VisionLayer makeLayer();

protected:
  using ScreenObject = std::pair<lv_obj_t*, lv_style_t>;

  /**
   * Allocate or return a previously allocated object.
   *
   * @return The allocated object.
   */
  ScreenObject& addObject();

  std::forward_list<ScreenObject> objects {};
  std::forward_list<ScreenObject>::iterator iterator {objects.before_begin()};
};

/**
 * A helper class to draw dynamically colored objects to the screen.
 */
class VisionLayer {
public:
  /**
   * Create a vision layer. Prefer using VisionPage::makeLayer. 
   *
   * @param idrawer The vision page
   */
  VisionLayer(VisionPage& idrawer);

  /**
   * Set a color for all objects drawn by this layer.
   *
   * @param  color The color
   */
  VisionLayer& withColor(const lv_color_t& color);

  /**
   * Set a color and border for all objects drawn by this layer.
   *
   * @param  color The color
   */
  VisionLayer& withColor(const lv_color_t& main, const lv_color_t& border);

  /**
   * Set a color for all objects of a specific signature drawn by this layer.
   *
   * @param  color The color
   * @param  sig   The signature
   */
  VisionLayer& withColor(const lv_color_t& color, uint16_t sig);

  /**
   * Set a color and border for all objects of a specific signature drawn by this layer.
   *
   * @param  color The color
   * @param  sig   The signature
   */
  VisionLayer& withColor(const lv_color_t& main, const lv_color_t& border, uint16_t sig);

  /**
   * Draw a container to the screen, using the predefined colors.
   *
   * @param container The container
   */
  void draw(const Vision::Container& container);

protected:
  VisionPage& drawer;
  using Style = std::pair<lv_color_t, lv_color_t>;
  Style defaultColor {LV_COLOR_BLACK, LV_COLOR_WHITE};
  std::map<uint16_t, Style> sigColors {};
};

} // namespace lib7842::GUI
