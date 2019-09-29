#include "pathPoint.hpp"

namespace lib7842 {

void PathPoint::setData(std::string iid, pathData_t idata) {
  pathData[iid] = idata;
}

PathPoint::pathData_t PathPoint::getData(std::string iid) {
  return pathData[iid];
}

double PathPoint::getCurvature() {
  pathData_t data = getData("curvature");
  try {
    return std::get<double>(data);
  } catch (const std::bad_variant_access& e) {
    if (std::get_if<std::monostate>(&data)) {
      return 0;
    } else {
      throw std::runtime_error("PathPoint::getCurvature:: \"curvature\" contains wrong type");
    }
  }
}

QLength PathPoint::getDistance() {
  pathData_t data = getData("distance");
  try {
    return std::get<QLength>(data);
  } catch (const std::bad_variant_access& e) {
    if (std::get_if<std::monostate>(&data)) {
      return 0_in;
    } else {
      throw std::runtime_error("PathPoint::getDistance:: \"distance\" contains wrong type");
    }
  }
}

QSpeed PathPoint::getVelocity() {
  pathData_t data = getData("velocity");
  try {
    return std::get<QSpeed>(data);
  } catch (const std::bad_variant_access& e) {
    if (std::get_if<std::monostate>(&data)) {
      return 0_mps;
    } else {
      throw std::runtime_error("PathPoint::getVelocity:: \"velocity\" contains wrong type");
    }
  }
}

int PathPoint::getSegmentIndex() {
  pathData_t data = getData("segmentIndex");
  try {
    return std::get<double>(data);
  } catch (const std::bad_variant_access& e) {
    if (std::get_if<std::monostate>(&data)) {
      return 0;
    } else {
      throw std::runtime_error("PathPoint::getSegmentIndex:: \"segmentIndex\" contains wrong type");
    }
  }
}

} // namespace lib7842