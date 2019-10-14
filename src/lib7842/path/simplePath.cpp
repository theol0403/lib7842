#include "simplePath.hpp"
#include "compoundPath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::initializer_list<Vector>& ipath) :
  SimplePath(std::vector<VectorRef>({ipath.begin(), ipath.end()})) {}

SimplePath::SimplePath(const std::vector<VectorRef>& ipath) {
  for (auto&& ipoint : ipath) {
    path.emplace_back(std::make_shared<Vector>(ipoint.get()));
  }
}

SimplePath::SimplePath(const std::vector<std::shared_ptr<Vector>>& ipath) : path(ipath) {}

std::vector<std::shared_ptr<Vector>>& SimplePath::get() {
  return path;
}

std::vector<std::shared_ptr<Vector>>& SimplePath::operator()() {
  return path;
}

SimplePath SimplePath::extract() const {
  return *this;
}

SimplePath SimplePath::extractCopy() const {
  SimplePath temp;
  temp().reserve(path.size());
  for (auto&& point : path) {
    temp().emplace_back(std::make_shared<Vector>(*point));
  }
  return temp;
}

ReferencePath SimplePath::extractRef() const {
  ReferencePath temp;
  temp().reserve(path.size());
  for (auto&& point : path) {
    temp().emplace_back(*point);
  }
  return temp;
}

std::shared_ptr<AbstractPath> SimplePath::copyPtr() const {
  return std::make_shared<SimplePath>(*this);
}

std::shared_ptr<AbstractPath> SimplePath::movePtr() const {
  return std::make_shared<SimplePath>(std::move(*this));
}

SimplePath SimplePath::generate(const size_t isteps) const {
  CompoundPath collector;
  for (size_t i = 0; i < path.size() - 1; i++) {
    collector.importPath(generateSegment(*path[i], *path[i + 1], isteps));
  }

  SimplePath temp = collector.extract();

  // push the last point
  if (path.size() > 0) temp().emplace_back(path.back());
  return temp;
}

SimplePath
  SimplePath::generateSegment(const Vector& start, const Vector& end, const size_t isteps) {
  SimplePath segment;
  Vector diff = end - start;

  // how much to increment each point
  Vector step = diff / isteps;

  // reserve vector capacity
  segment().reserve(segment().capacity() + isteps);

  for (size_t j = 0; j < isteps; j++) {
    segment().emplace_back(std::make_shared<Vector>(start + (step * j)));
  }

  return segment;
}

} // namespace lib7842