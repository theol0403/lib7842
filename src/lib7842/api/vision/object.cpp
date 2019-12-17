#include "object.hpp"

namespace lib7842::Vision {

Object::Object(const pros::vision_object& iobject) {
  sig = iobject.signature;
  x = iobject.left_coord;
  y = iobject.top_coord;
  width = iobject.width;
  height = iobject.height;
}

double Object::get(const Query& iquery) const {
  return iquery(*this);
}

} // namespace lib7842::Vision
