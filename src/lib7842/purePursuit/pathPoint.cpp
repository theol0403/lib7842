#include "pathPoint.hpp"

namespace lib7842 {

void PathPoint::setData(const std::string& iid, const pathData_t& idata) {
  pathData[iid] = idata;
}

PathPoint::pathData_t& PathPoint::getData(const std::string& iid) {
  return pathData[iid];
}

} // namespace lib7842