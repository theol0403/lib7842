// #include "dataPath.hpp"

// namespace lib7842 {

// DataPath::DataPath(const std::initializer_list<DataPoint>& ipath) : path(ipath) {}
// DataPath::DataPath(const std::vector<DataPoint>& ipath) : path(ipath) {}
// DataPath::DataPath(const AbstractPath& ipath) : DataPath() {
//   ReferencePath ref = ipath.extractRef();
//   path = {ref().begin(), ref().end()};
// }

// std::vector<DataPoint>& DataPath::get() {
//   return path;
// }

// std::vector<DataPoint>& DataPath::operator()() {
//   return path;
// }

// SimplePath DataPath::extract() const {
//   return SimplePath({path.begin(), path.end()});
// }

// ReferencePath DataPath::extractRef() const {
//   return ReferencePath({path.begin(), path.end()});
// }

// std::shared_ptr<AbstractPath> DataPath::copyPtr() const {
//   return std::make_shared<DataPath>(*this);
// }

// std::shared_ptr<AbstractPath> DataPath::movePtr() const {
//   return std::make_shared<DataPath>(std::move(*this));
// }

// } // namespace lib7842