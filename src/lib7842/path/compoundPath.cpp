#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const CompoundPath& ipath) : AbstractPath() {
  CompoundPath& ipathc = const_cast<CompoundPath&>(ipath);
  std::move(ipathc.path.begin(), ipathc.path.end(), std::back_inserter(path));
}

CompoundPath::CompoundPath(const AbstractPath& ipath) {
  addPath(ipath);
}

CompoundPath::CompoundPath(std::unique_ptr<AbstractPath> ipath) {
  addPath(std::move(ipath));
}

// CompoundPath& CompoundPath::addPath(const AbstractPath& ipath) {
//   addPath(std::make_unique<AbstractPath>(ipath));
//   return *this;
// }

CompoundPath& CompoundPath::addPath(std::unique_ptr<AbstractPath> ipath) {
  path.emplace_back(std::move(ipath));
  return *this;
}

SimplePath CompoundPath::extract() const {
  ReferencePath temp = extractRef();
  return SimplePath({temp.get().begin(), temp.get().end()});
}

ReferencePath CompoundPath::extractRef() const {
  std::vector<ReferencePath::PointRef> temp;
  for (auto&& segment : path) {
    for (auto&& ipoint : segment->extractRef().get()) {
      temp.emplace_back(ipoint);
    }
  }
  return ReferencePath(temp);
}

} // namespace lib7842