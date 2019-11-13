#pragma once

#include "page.hpp"
#include <map>

namespace lib7842 {

class ButtonMatrix : public Page {

public:
  using Page::Page;

  void initialize() override;
  static std::string getName();

  ButtonMatrix& makeButton(const std::string& iname, const std::function<void()>& iaction);
  ButtonMatrix& makeBreak();
  ButtonMatrix& build();

protected:
  static lv_res_t btnAction(lv_obj_t* ibtnm, const char* itxt);

  lv_obj_t* btnm {nullptr};
  lv_style_t bgStyle;
  lv_style_t relStyle;
  lv_style_t prStyle;
  lv_style_t inaStyle;

  std::vector<std::pair<std::string, std::function<void()>>> buttons {};
  std::vector<const char*> matrix {""};
};

} // namespace lib7842