#include "simplePath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::initializer_list<Vector>& ipath) :
  SimplePath(std::vector<Vector>(ipath)) {}

SimplePath::SimplePath(const std::vector<Vector>& ipath) {
  for (auto&& ipoint : ipath) {
    path.emplace_back(std::make_shared<Vector>(ipoint));
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
  std::vector<std::shared_ptr<Vector>> ipath;
  for (auto&& point : path) {
    ipath.emplace_back(std::make_shared<Vector>(*point));
  }
  return SimplePath(ipath);
}

SimplePath SimplePath::generate(const size_t isteps) const {}

// SimplePath LinearGenerator::insert(const AbstractPath& ipath, const QLength& ispacing) {
//   ReferencePath srcPath = ipath.extractRef();
//   SimplePath destPath;

//   for (size_t i = 0; i < srcPath().size() - 1; i++) {
//     const Vector& start = srcPath()[i].get();
//     const Vector& end = srcPath()[i + 1].get();
//     Vector diff = end - start;

//     // number of points needed
//     size_t numInsert = std::ceil((Vector::mag(diff) / ispacing).convert(number));
//     // reserve vector capacity
//     destPath().reserve(destPath().capacity() + numInsert);
//     // how much to increment each point
//     Vector step = diff / numInsert;

//     for (size_t j = 0; j < numInsert; j++) {
//       destPath().emplace_back(start + (step * j));
//     }
//   }

//   // push the last point
//   if (srcPath().size() > 0) destPath().emplace_back(srcPath().back());
//   return destPath;
// }

std::shared_ptr<AbstractPath> SimplePath::copyPtr() const {
  return std::make_shared<SimplePath>(*this);
}

std::shared_ptr<AbstractPath> SimplePath::movePtr() const {
  return std::make_shared<SimplePath>(std::move(*this));
}

} // namespace lib7842