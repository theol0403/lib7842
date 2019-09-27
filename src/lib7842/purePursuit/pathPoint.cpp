#include "pathPoint.hpp"

namespace lib7842 {

void PathPoint::setData(std::string iid, pathData_t idata) {
  pathData[iid] = idata;
}

PathPoint::pathData_t PathPoint::getData(std::string iid) {
  try {
    return pathData.at(iid);
  } catch(const std::out_of_range& e) { std::cerr << e.what() << std::endl; }
}

} // namespace lib7842