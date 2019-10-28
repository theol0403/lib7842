#include "simplePath.hpp"
#include "compoundPath.hpp"

namespace lib7842 {

SimplePath::SimplePath(const std::initializer_list<Vector>& ipath) :
  SimplePath(std::vector<Vector>(ipath)) {}

SimplePath::SimplePath(const std::vector<Vector>& ipath) {
  path.reserve(ipath.size());
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

const std::vector<std::shared_ptr<Vector>>& SimplePath::read() const {
  return path;
}

/**
 * Extract path containing copies of points
 */
SimplePath SimplePath::copy() const {
  SimplePath temp;
  temp().reserve(path.size());
  for (auto&& point : path) {
    temp().emplace_back(std::make_shared<Vector>(*point));
  }
  return temp;
}

/**
 * Smoothen path
 */
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

/**
 * Interpolate the path
 *
 * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
 * @return generated path
 */
SimplePath SimplePath::generate(const int isteps) const {
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

/**
 * Sample the segment
 * @param  isteps the number of points to generate in the segment excluding the end
 */
SimplePath SimplePath::generateSegment(const Vector& start, const Vector& end, const int isteps) {
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

} // namespace lib7842