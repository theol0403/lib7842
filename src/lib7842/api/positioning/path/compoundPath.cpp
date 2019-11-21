#include "compoundPath.hpp"

namespace lib7842 {

CompoundPath& CompoundPath::add(const std::shared_ptr<AbstractPath>& isegment) {
  segments.emplace_back(isegment);
  return *this;
}

SimplePath CompoundPath::generate(const int isteps) const {
  SimplePath temp;
  for (auto&& segment : segments) {
    SimplePath isegment = segment->generate(isteps);
    temp().reserve(temp().size() + isegment().size());
    for (auto&& point : isegment()) {
      temp().emplace_back(point);
    }
  }
  return temp;
}
} // namespace lib7842