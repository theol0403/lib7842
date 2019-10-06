#include "AnnotatedPoint.hpp"

namespace lib7842 {

void AnnotatedPoint::setData(const std::string& iid, const pathData_t& idata) {
  pathData[iid] = idata;
}

const AnnotatedPoint::pathData_t& AnnotatedPoint::getID(const std::string& iid) const {
  try {
    return pathData.at(iid);
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("AnnotatedPoint::getID:: \"" + iid + "\" does not exist in point");
  }
}

} // namespace lib7842