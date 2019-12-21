#include "graph.hpp"

namespace lib7842::GUI {

void Graph::initialize() {
  graph = lv_chart_create(container, NULL);
  lv_obj_set_size(graph, lv_obj_get_width(container), lv_obj_get_height(container));
  lv_obj_align(graph, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_chart_set_type(graph, LV_CHART_TYPE_LINE);
  lv_chart_set_series_opa(graph, LV_OPA_80);
  lv_chart_set_series_width(graph, 2);

  lv_style_copy(&gStyle, &lv_style_pretty);
  gStyle.body.main_color = themeColor;
  gStyle.body.grad_color = themeColor;
  gStyle.line.color = LV_COLOR_HEX(0xFFFCF9);
  gStyle.line.width = 2;
  lv_obj_set_style(graph, &gStyle);
}

void Graph::render() {
  for (auto&& [ser, data, style] : series) {
    (void)style;
    lv_chart_set_next(graph, ser, data());
  }
}

std::string Graph::getName() {
  return "Graph";
}

Graph& Graph::withRange(int imin, int imax) {
  lv_chart_set_range(graph, imin, imax);
  return *this;
}

Graph& Graph::withResolution(int iresolution) {
  lv_chart_set_point_count(graph, iresolution);
  return *this;
}

Graph& Graph::withGrid(int ihor, int iver) {
  lv_chart_set_div_line_count(graph, ihor, iver);
  return *this;
}

Graph& Graph::withSeries(const std::string& iname, const lv_color_t& icolor,
                         const std::function<double()>& idata) {
  lv_chart_series_t* ser = lv_chart_add_series(graph, icolor);
  lv_chart_init_points(graph, ser, idata());

  //Add text to legend
  lv_obj_t* label = lv_label_create(graph, NULL);
  lv_label_set_text(label, (std::string(SYMBOL_MINUS) + " " + iname).c_str());

  auto style = std::make_shared<lv_style_t>();
  lv_style_copy(style.get(), &lv_style_plain);
  style->text.color = icolor;
  style->text.opa = LV_OPA_100;
  style->text.font = &lv_font_dejavu_20;
  style->text.letter_space = 1;
  lv_obj_set_style(label, style.get());
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 7, 5 + lv_obj_get_height(label) * series.size());

  series.push_back(std::make_tuple(ser, idata, std::move(style)));
  return *this;
}

} // namespace lib7842::GUI