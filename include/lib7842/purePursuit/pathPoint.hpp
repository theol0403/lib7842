#pragma once
#include "main.h"
#include "point.hpp"
#include <variant>

namespace lib7842 {

class PathPoint : public QPoint {

  using QPoint::QPoint;
  using pathData_t = std::variant<std::monostate, double, QLength, QSpeed, QAcceleration>;

 public:
  void setData(std::string iid, pathData_t idata);
  pathData_t getData(std::string iid);

  template <typename T> T getValue(std::string iid) {
    pathData_t data = getData(iid);
    try {
      return std::get<T>(data);
    } catch (const std::bad_variant_access& e) {
      if (std::get_if<std::monostate>(&data)) {
        return T(0.0);
      } else {
        throw std::runtime_error("PathPoint::getValue:: \"" + iid + "\" contains wrong type");
      }
    }
  }

 protected:
  std::map<std::string, pathData_t> pathData {};
};

} // namespace lib7842