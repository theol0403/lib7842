#pragma once
#include "main.h"
#include "point.hpp"

#include "lib7842/other/utility.hpp"
#include <variant>

namespace lib7842 {

class AnnotatedPoint : public QPoint {

 public:
  using pathData_t =
    std::variant<std::monostate, double, QLength, QSpeed, QAcceleration, QCurvature>;

  using QPoint::QPoint;
  AnnotatedPoint(const AnnotatedPoint& istate) = default;
  virtual ~AnnotatedPoint() = default;

  void setData(const std::string& iid, const pathData_t& idata);

  template <typename T> T getData(const std::string& iid) const {
    const pathData_t& data = getID(iid);
    if (std::holds_alternative<T>(data)) {
      return std::get<T>(data);
    } else if (std::holds_alternative<std::monostate>(data)) {
      return T(0.0);
    } else {
      throw std::runtime_error("AnnotatedPoint::getValue:: \"" + iid + "\" contains wrong type");
    }
  }

 protected:
  std::map<std::string, pathData_t> pathData {};

 private:
  const pathData_t& getID(const std::string& iid) const;
};

} // namespace lib7842