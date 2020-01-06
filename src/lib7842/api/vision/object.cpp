#include "object.hpp"

namespace lib7842::Vision {

double Object::get(const Query& iquery) const {
  return iquery(*this);
}

} // namespace lib7842::Vision
