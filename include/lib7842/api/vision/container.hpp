#pragma once
#include "object.hpp"
#include <functional>

namespace lib7842::Vision {

class Container {
public:
  Container() = default;

  std::vector<Object>& operator()();
  const std::vector<Object>& operator()() const;

  Container& add(const Object& iobj);
  Container& add(const std::vector<Object>& iobjects);

  Container& remove(size_t index);
  Container& remove(size_t startIndex, size_t endIndex);

  using Compare = std::function<bool(double, double)>;

  Container& remove(const Query& query, const Compare& comp, double value);
  Container& remove(const Query& query, const std::vector<std::pair<Compare, double>>& comps);

  Container& sort(const Query& query, const Compare& comp);

  Container& resize(size_t size);
  Container& trim(size_t size);
  Container& reset();

  Object get(size_t index);
  Object total();
  Object avg();
  Container operator+(Container);

protected:
  std::vector<Object> objects = {};
};
} // namespace lib7842::Vision
