#include "simplePath.hpp"
#include "compoundPath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::initializer_list<Vector>& ipath) :
  SimplePath(std::vector<Vector>(ipath)) {}

SimplePath::SimplePath(const std::vector<Vector>& ipath) {
  for (auto&& ipoint : ipath) {
    path.emplace_back(std::make_shared<Vector>(ipoint));
  }
}

SimplePath::SimplePath(const std::vector<std::shared_ptr<Vector>>& ipath) : path(ipath) {}

std::vector<std::shared_ptr<Vector>>& SimplePath::get() {
  return path;
}

std::vector<std::shared_ptr<Vector>>& SimplePath::operator()() {
  return path;
}

VectorRef SimplePath::operator[](const size_t iindex) {
  return *path.at(iindex);
}

SimplePath SimplePath::copy() const {
  SimplePath temp;
  temp().reserve(path.size());
  for (auto&& point : path) {
    temp().emplace_back(std::make_shared<Vector>(*point));
  }
  return temp;
}

// ReferencePath SimplePath::ref() const {
//   ReferencePath temp;
//   temp().reserve(path.size());
//   for (auto&& point : path) {
//     temp().emplace_back(*point);
//   }
//   return temp;
// }

void SimplePath::smoothen(const double iweight, const QLength& itolerance) {
  SimplePath destPath = this->copy();

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

SimplePath SimplePath::generate(const int isteps) const {
  if (isteps < 1) throw std::runtime_error("SimplePath::generate: isteps is less than 1");

  SimplePath temp;

  // if there is a segment to interpolate
  if (path.size() > 1) {
    // for each pair of points
    for (size_t i = 0; i < path.size() - 1; i++) {
      // if interpolation needed
      if (isteps > 1) {
        // if we are on last segment, skip last point
        bool lastSegment = i == path.size() - 2;
        // generate segment
        SimplePath segment =
          generateSegment(*path[i], *path[i + 1], lastSegment ? isteps - 1 : isteps);
        // move segment into path
        std::move(segment().begin(), segment().end(), std::back_inserter(temp()));
      } else {
        // push back point
        temp().emplace_back(std::make_shared<Vector>(*path[i]));
      }
    }
  }
  // push the last point
  if (path.size() > 0) temp().emplace_back(path.back());
  return temp;
}

std::shared_ptr<AbstractPath> SimplePath::copyPtr() const {
  return std::make_shared<SimplePath>(*this);
}

std::shared_ptr<AbstractPath> SimplePath::movePtr() const {
  return std::make_shared<SimplePath>(std::move(*this));
}

SimplePath SimplePath::generateSegment(const Vector& start, const Vector& end, const int isteps) {
  if (isteps < 1) throw std::runtime_error("SimplePath::generateSegment: isteps is less than 1");
  SimplePath segment;
  Vector diff = end - start;

  // how much to increment each point
  Vector step = diff / isteps;
  // reserve vector capacity
  segment().reserve(isteps);

  for (size_t i = 0; i < isteps; i++) {
    segment().emplace_back(std::make_shared<Vector>(start + (step * i)));
  }

  return segment;
}

} // namespace lib7842