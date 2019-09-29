#pragma once
#include "main.h"
#include "point.hpp"
#include <variant>

namespace lib7842 {

class PathPoint : public QPoint {

  using QPoint::QPoint;
  using pathData_t = std::variant<std::monostate, double, QLength, QSpeed, QAcceleration>;

 public:
  void setData(const std::string& iid, const pathData_t& idata);
  pathData_t& getData(const std::string& iid);

  template <typename T> T getValue(const std::string& iid) {
    pathData_t& data = getData(iid);
    if (std::holds_alternative<T>(data)) {
      return std::get<T>(data);
    } else if (std::holds_alternative<std::monostate>(data)) {
      return T(0.0);
    } else {
      throw std::runtime_error("PathPoint::getValue:: \"" + iid + "\" contains wrong type");
    }
  }

 protected:
  std::map<std::string, pathData_t> pathData {};
};

} // namespace lib7842