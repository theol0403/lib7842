#include "lib7842/api/vision/query.hpp"
#include "lib7842/api/vision/object.hpp"

namespace lib7842::Vision {

Query Query::sig = [](const Object& obj) {
  return obj.sig;
};

Query Query::x = [](const Object& obj) {
  return obj.x;
};

Query Query::y = [](const Object& obj) {
  return obj.y;
};

Query Query::width = [](const Object& obj) {
  return obj.width;
};

Query Query::height = [](const Object& obj) {
  return obj.height;
};

Query Query::fovWidth = [](const Object& obj) {
  return obj.fovWidth;
};

Query Query::fovHeight = [](const Object& obj) {
  return obj.fovHeight;
};

Query Query::area = [](const Object& obj) {
  return obj.width * obj.height;
};

Query Query::avgDim = [](const Object& obj) {
  return (obj.width + obj.height) / 2.0;
};

Query Query::centerX = [](const Object& obj) {
  return obj.x + obj.width / 2.0;
};

Query Query::centerY = [](const Object& obj) {
  return obj.y + obj.height / 2.0;
};

double Query::getFrom(const Object& obj) const { return (*this)(obj); }

} // namespace lib7842::Vision
