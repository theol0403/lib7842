#include "pathPoint.hpp"

namespace lib7842 {

void PathPoint::setData(std::string iid, pathData_t idata) {
  pathData[iid] = idata;
}

PathPoint::pathData_t PathPoint::getData(std::string iid) {
  try {
    return pathData.at(iid);
  } catch(const std::out_of_range& e) {
    std::cerr << "PathPoint::getData: " << e.what() << std::endl;
    return pathData_t();
  }
}

double PathPoint::getCurvature() {
  try {
    return std::get<double>(getData("curvature"));
  } catch(const std::bad_variant_access& e) { return 0; }
}

QLength PathPoint::getDistance() {
  try {
    return std::get<QLength>(getData("distance"));
  } catch(const std::bad_variant_access& e) { return 0_in; }
}

QSpeed PathPoint::getVelocity() {
  try {
    return std::get<QSpeed>(getData("velocity"));
  } catch(const std::bad_variant_access& e) { return 0_mps; }
}

int PathPoint::getSegmentIndex() {
  try {
    return std::get<double>(getData("segmentIndex"));
  } catch(const std::bad_variant_access& e) { return 0; }
}

} // namespace lib7842