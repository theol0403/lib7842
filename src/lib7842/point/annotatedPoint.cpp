#include "AnnotatedPoint.hpp"

namespace lib7842 {

void AnnotatedPoint::setData(const std::string& iid, const pathData_t& idata) {
  pathData[iid] = idata;
}

AnnotatedPoint::pathData_t& AnnotatedPoint::getID(const std::string& iid) {
  return pathData[iid];
}

} // namespace lib7842