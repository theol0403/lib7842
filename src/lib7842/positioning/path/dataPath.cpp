#include "dataPath.hpp"

namespace lib7842 {

DataPath::DataPath(const std::initializer_list<DataPoint>& ipath) :
  DataPath(std::vector<DataPoint>(ipath)) {}

DataPath::DataPath(const std::vector<DataPoint>& ipath) {
  path.reserve(ipath.size());
  for (auto&& ipoint : ipath) {
    path.emplace_back(std::make_shared<DataPoint>(ipoint));
  }
}

DataPath::DataPath(const std::vector<std::shared_ptr<DataPoint>>& ipath) : path(ipath) {}

DataPath::DataPath(const SimplePath& ipath) {
  // copy from source path to data path
  path.reserve(ipath.read().size());
  std::transform(
    ipath.read().begin(), ipath.read().end(), std::back_inserter(path),
    [](const std::shared_ptr<Vector>& ipoint) {
      return std::make_shared<DataPoint>(*ipoint);
    });
}

std::vector<std::shared_ptr<DataPoint>>& DataPath::get() {
  return path;
}

std::vector<std::shared_ptr<DataPoint>>& DataPath::operator()() {
  return path;
}

const std::vector<std::shared_ptr<DataPoint>>& DataPath::read() const {
  return path;
}

SimplePath DataPath::generate(const int isteps) const {
  return SimplePath(std::vector<std::shared_ptr<Vector>>({path.begin(), path.end()}))
    .generate(isteps);
}

std::shared_ptr<AbstractPath> DataPath::copyPtr() const {
  return std::make_shared<DataPath>(*this);
}

std::shared_ptr<AbstractPath> DataPath::movePtr() const {
  return std::make_shared<DataPath>(std::move(*this));
}

} // namespace lib7842