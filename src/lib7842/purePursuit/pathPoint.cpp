#include "pathPoint.hpp"

namespace lib7842 {

void PathPoint::setData(std::string iid, pathData_t idata) {
  pathData[iid] = idata;
}

PathPoint::pathData_t PathPoint::getData(std::string iid) {
  return pathData[iid];
}

} // namespace lib7842