#pragma once

#include "page.hpp"
#include <tuple>

namespace lib7842::GUI {

class Graph : public Page {
public:
  using Page::Page;

  /**
   * Initialize the graph.
   */
  void initialize() override;

  /**
   * Render the graph.
   */
  void render() override;

  /**
   * Get the default tab name.
   *
   * @return The name.
   */
  static std::string getName();

  /**
   * Set the graph range.
   *
   * @param  imin The minimum value
   * @param  imax The maximum value
   */
  Graph& withRange(int imin, int imax);

  /**
   * Set the graph resolution (number of data points on the graph)
   *
   * @param  iresolution The resolution of the graph.
   */
  Graph& withResolution(int iresolution);

  /**
   * Set the number of grid lines.
   *
   * @param  ihor The number of horizontal grid lines.
   * @param  iver The number of vertical grid lines.
   */
  Graph& withGrid(int ihor, int iver);

  /**
   * Add a graph series. Specify the name, color, and pass a function that will return the graph
   * data.
   *
   * @param  iname  The series name
   * @param  icolor The series color
   * @param  idata  The data function
   */
  Graph& withSeries(const std::string& iname,
                    const lv_color_t& icolor,
                    const std::function<double()>& idata);

private:
  lv_obj_t* graph {nullptr};
  lv_style_t gStyle;

  std::vector<std::tuple<lv_chart_series_t*, std::function<double()>, std::shared_ptr<lv_style_t>>>
    series = {};
};

} // namespace lib7842::GUI