#pragma once

#include "buttonMatrix.hpp"

namespace lib7842 {

class AutonSelector : public ButtonMatrix {
public:
  using ButtonMatrix::ButtonMatrix;

  void initialize() override;
  static std::string getName();

  void run();

protected:
  static lv_res_t btnAction(lv_obj_t* ibtnm, const char* itxt);

  int currentIndex {0};
};

} // namespace lib7842