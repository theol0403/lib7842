#include "lib7842/api/positioning/path/compoundPath.hpp"

namespace lib7842 {

CompoundPath& CompoundPath::add(const std::shared_ptr<AbstractPath>& isegment) {
  segments.emplace_back(isegment);
  return *this;
}

CompoundPath& CompoundPath::operator+=(const std::shared_ptr<AbstractPath>& isegment) {
  segments.emplace_back(isegment);
  return *this;
}

CompoundPath CompoundPath::operator+(const std::shared_ptr<AbstractPath>& isegment) & {
  return CompoundPath(*this) + isegment;
}

CompoundPath&& CompoundPath::operator+(const std::shared_ptr<AbstractPath>& isegment) && {
  segments.emplace_back(isegment);
  return std::move(*this);
}

SimplePath CompoundPath::combine() const {
  SimplePath temp;
  for (auto&& segment : segments) {
    SimplePath isegment = segment->generate(1);
    temp().reserve(temp().size() + isegment().size());
    for (auto&& point : isegment()) {
      temp().emplace_back(point);
    }
  }
  return temp;
}

SimplePath CompoundPath::generate(int isteps, bool iend) const {
  SimplePath temp;
  for (size_t i = 0; i < segments.size(); i++) {
    SimplePath isegment = segments[i]->generate(isteps, iend && i == segments.size() - 1);
    temp().reserve(temp().size() + isegment().size());
    for (auto&& point : isegment()) {
      temp().emplace_back(point);
    }
  }
  return temp;
}

} // namespace lib7842
