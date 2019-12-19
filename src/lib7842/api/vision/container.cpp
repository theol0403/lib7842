#include "container.hpp"
#include <numeric>

namespace lib7842::Vision {

std::vector<Object>& Container::operator()() {
  return objects;
}

const std::vector<Object>& Container::operator()() const {
  return objects;
}

Container& Container::add(const Object& iobj) {
  objects.emplace_back(iobj);
  return *this;
}

Container& Container::add(const std::vector<Object>& iobjects) {
  objects.reserve(objects.size() + iobjects.size());
  objects.insert(objects.end(), iobjects.begin(), iobjects.end());
  return *this;
}

Container& Container::remove(size_t index) {
  if (index < objects.size()) objects.erase(objects.begin() + index);
  return *this;
}

Container& Container::remove(size_t startIndex, size_t endIndex) {
  if (endIndex > objects.size()) endIndex = objects.size();
  if (startIndex < objects.size()) {
    objects.erase(objects.begin() + startIndex, objects.begin() + endIndex);
  }
  return *this;
}

Object Container::get(size_t index) {
  if (index < objects.size()) {
    return objects.at(index);
  } else {
    return {};
  }
}

double Container::get(size_t index, const Query& query) {
  return get(index).get(query);
}

double Container::total(const Query& query) {
  return std::accumulate(objects.begin(), objects.end(), 0.0,
                         [&](double a, const Object& b) { return a + query(b); });
}

double Container::avg(const Query& query) {
  return total(query) / static_cast<double>(objects.size());
}

Container& Container::resize(size_t size) {
  objects.resize(size);
  return *this;
}

Container& Container::trim(size_t size) {
  if (size < objects.size()) resize(size);
  return *this;
}

Container& Container::reset() {
  resize(0);
  return *this;
}

Container& Container::remove(const Query& query, const Compare& comp, double value) {
  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [&](const Object& obj) { return comp(query(obj), value); }),
                objects.end());
  return *this;
}

Container& Container::remove(const std::function<bool(const Object&)>& comp) {
  objects.erase(std::remove_if(objects.begin(), objects.end(), comp), objects.end());
  return *this;
}

Container& Container::sort(const Query& query, const Compare& comp) {
  std::stable_sort(objects.begin(), objects.end(),
                   [&](const Object& a, const Object& b) { return comp(query(a), query(b)); });
  return *this;
}

} // namespace lib7842::Vision