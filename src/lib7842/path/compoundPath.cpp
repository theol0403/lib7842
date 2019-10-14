// #include "compoundPath.hpp"

// namespace lib7842 {

// CompoundPath::CompoundPath(const std::shared_ptr<AbstractPath>& ipath) {
//   addPath(std::move(ipath));
// }

// CompoundPath& CompoundPath::addPath(const std::shared_ptr<AbstractPath>& ipath) {
//   paths.emplace_back(std::move(ipath));
//   return *this;
// }

// CompoundPath& CompoundPath::copyPath(const AbstractPath& ipath) {
//   addPath(std::move(ipath.copyPtr()));
//   return *this;
// }

// CompoundPath& CompoundPath::importPath(const AbstractPath& ipath) {
//   addPath(std::move(ipath.movePtr()));
//   return *this;
// }

// SimplePath CompoundPath::extract() const {
//   ReferencePath temp = extractRef();
//   return SimplePath({temp.get().begin(), temp.get().end()});
// }

// ReferencePath CompoundPath::extractRef() const {
//   std::vector<PointReference> temp;
//   for (auto&& path : paths) {
//     ReferencePath ref = path->extractRef();
//     for (auto&& point : ref.get()) {
//       temp.emplace_back(point);
//     }
//   }
//   return ReferencePath(temp);
// }

// std::shared_ptr<AbstractPath> CompoundPath::copyPtr() const {
//   return std::make_shared<CompoundPath>(*this);
// }

// std::shared_ptr<AbstractPath> CompoundPath::movePtr() const {
//   return std::make_shared<CompoundPath>(std::move(*this));
// }

// } // namespace lib7842