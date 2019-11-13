#pragma once

#include "page.hpp"
#include <string>
#include <tuple>

namespace lib7842 {

class Graph : public Page {

public:
  using Page::Page;

  void initialize() override;
  void render() override;

  static std::string getName();

  Graph& withRange(int imin, int imax);
  Graph& withResolution(int iresolution);
  Graph& withGrid(int ihor, int iver);
  Graph& withRefreshRate(int iminDelta);

  using Data = std::function<double()>;

  Graph& withSeries(const std::string& iname, const lv_color_t& icolor, const Data& idata);

protected:
  lv_obj_t* graph {nullptr};
  lv_style_t gStyle;

  int minDelta {0};

  using Entry = std::tuple<lv_chart_series_t*, Data, std::shared_ptr<lv_style_t>>;
  std::vector<Entry> series = {};
};

} // namespace lib7842