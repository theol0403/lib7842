#include "packedPath.hpp"

namespace lib7842 {

PackedPath::PackedPath(const std::initializer_list<PackedPoint>& ipath) : path(ipath) {}
PackedPath::PackedPath(const std::vector<PackedPoint>& ipath) : path(ipath) {}
PackedPath::PackedPath(const AbstractPath& ipath) : PackedPath(ipath.extractRef()()) {}

std::vector<PackedPoint>& PackedPath::get() {
  return path;
}

std::vector<PackedPoint>& PackedPath::operator()() {
  return path;
}

SimplePath PackedPath::extract() const {
  return SimplePath({path.begin(), path.end()});
}

ReferencePath PackedPath::extractRef() const {
  return ReferencePath({path.begin(), path.end()});
}

std::shared_ptr<AbstractPath> PackedPath::copyPtr() const {
  return std::make_shared<PackedPath>(*this);
}

std::shared_ptr<AbstractPath> PackedPath::movePtr() const {
  return std::make_shared<PackedPath>(std::move(*this));
}

PackedPath::PackedPath(const std::vector<PointReference>& ipath) :
  PackedPath(std::vector<PackedPoint> {ipath.begin(), ipath.end()}) {}

} // namespace lib7842