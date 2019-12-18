#pragma once
#include "object.hpp"
#include <functional>

namespace lib7842::Vision {

class Container {
public:
  Container() = default;
  void add(const Object& obj);
  void add(const std::vector<Object>& objects);

  std::vector<Object>& operator()(size_t index);
  const std::vector<Object>& operator()(size_t index) const;

  Container& resize(size_t size);
  Container& trim(size_t size);

  void reset();

  Container& remove(size_t index);
  Container& remove(size_t startIndex, size_t endIndex);

  using Compare = std::function<bool(double, double)>;

  void remove(const Query& query, const Compare& comp, double value);
  void remove(const Query& query, const std::vector<std::pair<Compare, double>>);

  Container& sort(const Query& query, const Compare& comp);

  Object total();
  Object avg();
  Container operator+(Container);

protected:
  std::vector<Object> objects = {};
};
} // namespace lib7842::Vision
