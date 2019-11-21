#pragma once

#include "actions.hpp"

namespace lib7842::GUI {

class Selector : public Actions {
public:
  using Actions::Actions;

  void initialize() override;
  static std::string getName();

  void run();

protected:
  static lv_res_t btnAction(lv_obj_t* ibtnm, const char* itxt);

  int currentIndex {0};
};

} // namespace lib7842::GUI