#include "compoundPath.hpp"

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

CompoundPath::operator std::shared_ptr<AbstractPath>() & {
  return std::make_shared<CompoundPath>(*this);
}

CompoundPath::operator std::shared_ptr<AbstractPath>() && {
  return std::make_shared<CompoundPath>(std::move(*this));
}

} // namespace lib7842