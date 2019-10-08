#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const AbstractPath& ipath) {
  addPath(ipath);
}

CompoundPath::CompoundPath(const std::shared_ptr<AbstractPath>& ipath) {
  addPath(std::move(ipath));
}

CompoundPath& CompoundPath::addPath(const AbstractPath& ipath) {
  addPath(std::move(ipath.copyPtr()));
  return *this;
}

CompoundPath& CompoundPath::addPath(const std::shared_ptr<AbstractPath>& ipath) {
  paths.emplace_back(std::move(ipath));
  return *this;
}

SimplePath CompoundPath::extract() const {
  ReferencePath temp = extractRef();
  return SimplePath({temp.get().begin(), temp.get().end()});
}

ReferencePath CompoundPath::extractRef() const {
  std::vector<ReferencePath::PointRef> temp;
  for (auto&& path : paths) {
    ReferencePath ref = path->extractRef();
    for (auto&& point : ref.get()) {
      temp.emplace_back(point);
    }
  }
  return ReferencePath(temp);
}

std::shared_ptr<AbstractPath> CompoundPath::copyPtr() const {
  return std::make_shared<CompoundPath>(*this);
}

} // namespace lib7842