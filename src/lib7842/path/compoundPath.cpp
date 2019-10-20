#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath::CompoundPath(const std::shared_ptr<AbstractPath>& ipath) {
  addPath(std::move(ipath));
}

CompoundPath& CompoundPath::addPath(const std::shared_ptr<AbstractPath>& ipath) {
  paths.emplace_back(std::move(ipath));
  return *this;
}

CompoundPath& CompoundPath::copyPath(const AbstractPath& ipath) {
  addPath(std::move(ipath.copyPtr()));
  return *this;
}

CompoundPath& CompoundPath::importPath(const AbstractPath& ipath) {
  addPath(std::move(ipath.movePtr()));
  return *this;
}

SimplePath CompoundPath::generate(const int isteps) const {
  SimplePath temp;
  std::cout << paths.size() << std::endl;
  for (auto&& path : paths) {
    SimplePath ipath = path->generate(isteps);
    std::cout << ipath().size() << std::endl;
    temp().reserve(temp().size() + ipath().size());
    for (auto&& point : ipath()) {
      temp().emplace_back(point);
    }
  }
  return temp;
}

std::shared_ptr<AbstractPath> CompoundPath::copyPtr() const {
  return std::make_shared<CompoundPath>(*this);
}

std::shared_ptr<AbstractPath> CompoundPath::movePtr() const {
  return std::make_shared<CompoundPath>(std::move(*this));
}

} // namespace lib7842