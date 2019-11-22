#include "discretePath.hpp"
#include "compoundPath.hpp"

namespace lib7842 {

template <typename T> DiscretePath<T>::DiscretePath(const std::initializer_list<T>& ipath) {
  path.reserve(ipath.size());
  std::transform(ipath.begin(), ipath.end(), std::back_inserter(path), [](const auto& ipoint) {
    return std::make_shared<T>(ipoint);
  });
}

template <typename T> DiscretePath<T>::DiscretePath(const std::vector<T>& ipath) {
  path.reserve(ipath.size());
  std::transform(ipath.begin(), ipath.end(), std::back_inserter(path), [](const auto& ipoint) {
    return std::make_shared<T>(ipoint);
  });
}

template <typename T>
DiscretePath<T>::DiscretePath(const std::vector<std::shared_ptr<T>>& ipath) : path(ipath) {}

template <typename T> std::vector<std::shared_ptr<T>>& DiscretePath<T>::get() {
  return path;
}

template <typename T> std::vector<std::shared_ptr<T>>& DiscretePath<T>::operator()() {
  return path;
}

template <typename T> const std::vector<std::shared_ptr<T>>& DiscretePath<T>::read() const {
  return path;
}

template <typename T> DiscretePath<T> DiscretePath<T>::copy() const {
  DiscretePath<T> temp;
  temp().reserve(path.size());
  for (auto&& point : path) {
    temp().emplace_back(std::make_shared<T>(*point));
  }
  return temp;
}

template <typename T> void DiscretePath<T>::smoothen(const double iweight, const QLength& itolerance) {
  DiscretePath<T> destPath = copy();

  double weight = 1.0 - iweight;
  QLength change = itolerance;

  while (change >= itolerance) {
    change = 0.0_in;
    for (size_t i = 1; i < path.size() - 1; i++) {
      for (size_t j = 0; j < 2; j++) {
        QLength& destPoint = destPath()[i]->at(j);
        QLength dataFac = iweight * (path[i]->read(j) - destPoint);
        QLength smoothFac =
          weight * (destPath()[i - 1]->read(j) + path[i + 1]->read(j) - (2.0 * destPoint));
        destPoint += (dataFac + smoothFac);
        change = (destPoint - destPath()[i]->read(j)).abs();
      }
    }
  }
  path = std::move(destPath.path);
}

template <typename T> SimplePath DiscretePath<T>::generate(const int isteps) const {
  if (isteps < 1) throw std::runtime_error("SimplePath::generate: isteps is less than 1");

  SimplePath temp;
  if (path.size() > 0) temp().reserve((isteps * (path.size() - 1)) + 1);

  // if path is more than 2 points - interpolation needed
  if (path.size() > 1) {
    // for each segment
    for (size_t i = 0; i < path.size() - 1; i++) {
      // if interpolation needed
      if (isteps > 1) {
        // generate segment
        SimplePath segment = generateSegment(*path[i], *path[i + 1], isteps);
        // move segment into path
        std::move(segment().begin(), segment().end(), std::back_inserter(temp()));
      } else {
        // interpolation not needed
        temp().emplace_back(std::make_shared<Vector>(*path[i]));
      }
    }
  }

  // if path is more than 1 point - return last point
  if (path.size() > 0) temp().emplace_back(path.back());
  return temp;
}

template <typename T>
SimplePath DiscretePath<T>::generateSegment(const Vector& start, const Vector& end, const int isteps) {
  if (isteps < 1) throw std::runtime_error("SimplePath::generateSegment: isteps is less than 1");
  SimplePath segment;

  Vector diff = end - start;
  Vector step = diff / isteps;
  // reserve vector capacity
  segment().reserve(isteps);

  for (size_t i = 0; i < isteps; i++) {
    segment().emplace_back(std::make_shared<Vector>(start + (step * i)));
  }

  return segment;
}

template class DiscretePath<Vector>;
template class DiscretePath<DataPoint>;
template class DiscretePath<State>;

} // namespace lib7842