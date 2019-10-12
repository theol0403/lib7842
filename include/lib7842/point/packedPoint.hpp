#pragma once
#include "main.h"
#include "point.hpp"

#include "lib7842/other/utility.hpp"
#include <any>

namespace lib7842 {

class PackedPoint : public Vector {

 public:
  using Vector::Vector;
  PackedPoint(const PackedPoint& istate) = default;
  virtual ~PackedPoint() = default;

  explicit PackedPoint(const Vector& ipoint);

  void setData(const std::string& iid, const std::any& idata);

  template <typename T> T getData(const std::string& iid) const {
    const std::any& data = getID(iid);
    try {
      return std::any_cast<T>(data);
    } catch (const std::bad_any_cast& e) {
      throw std::runtime_error(
        "PackedPoint::getData:: \"" + iid + "\" contains wrong type \"" + data.type().name()
        + "\"");
    }
  }

 protected:
  std::map<std::string, std::any> pathData {};

 private:
  const std::any& getID(const std::string& iid) const;
};

} // namespace lib7842