#include "packedPoint.hpp"

namespace lib7842 {

void PackedPoint::setData(const std::string& iid, const std::any& idata) {
  pathData[iid] = idata;
}

const std::any& PackedPoint::getID(const std::string& iid) const {
  try {
    return pathData.at(iid);
  } catch (const std::out_of_range& e) {
    throw std::runtime_error("PackedPoint::getID:: \"" + iid + "\" does not exist in point");
  }
}

} // namespace lib7842