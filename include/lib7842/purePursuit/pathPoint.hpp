#pragma once
#include "main.h"
#include "point.hpp"
#include <variant>

namespace lib7842 {

class PathPoint : public QPoint {

  using pathData_t = std::variant<double, QLength, QSpeed>;

  public:
  void setData(std::string iid, pathData_t idata);
  pathData_t getData(std::string iid);

  protected:
  std::map<std::string, pathData_t> pathData {};
};

} // namespace lib7842