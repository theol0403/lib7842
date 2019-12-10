#include "dataPoint.hpp"

namespace lib7842 {

DataPoint::DataPoint(const Vector& ipoint) : Vector(ipoint) {};

void DataPoint::setData(const std::string& iid, const std::any& idata) {
  data[iid] = idata;
}

const std::any& DataPoint::getID(const std::string& iid) const {
  try {
    return data.at(iid);
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("DataPoint::getID:: \"" + iid + "\" does not exist in point");
  }
}

} // namespace lib7842