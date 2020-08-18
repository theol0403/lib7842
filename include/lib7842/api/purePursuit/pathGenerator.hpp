#pragma once
#include "lib7842/api/positioning/path/path.hpp"
#include "lib7842/api/positioning/path/stepper.hpp"
#include "pursuitLimits.hpp"
#include "waypoint.hpp"

namespace lib7842 {

class PathGenerator {
public:
  /**
   * Generate a PursuitPath containing waypoint information for pure pursuit.
   *
   * @param  ip  The path
   * @param  limits The pure pursuit limits
   * @return the generated path
   */
  template <typename T, typename U, typename S>
  requires(!ConstStepper<S>) static std::vector<Waypoint> generate(const Stepper<T, U, S>& ip,
                                                                   const PursuitLimits& limits) {
    return generate(ip.generate(), limits);
  }

  template <size_t N>
  static std::vector<Waypoint> generate(const std::array<State, N>& ip,
                                        const PursuitLimits& limits) {
    return generate({ip.begin(), ip.end()}, limits);
  }

  /**
   * Generate a PursuitPath containing waypoint information for pure pursuit.
   *
   * @param  ip      The path
   * @param  limits  The pure pursuit limits
   * @return the generated path
   */
  static std::vector<Waypoint> generate(const std::vector<State>& ip, const PursuitLimits& limits) {
    std::vector<Waypoint> path;
    path.reserve(ip.size());
    std::transform(ip.begin(), ip.end(), std::back_inserter(path),
                   [](const State& p) { return Waypoint(p); });
    setCurvatures(path);
    setVelocity(path, limits);
    return path;
  }

protected:
  /**
   * Sets the waypoint curvatures.
   *
   * @param ipath The path
   */
  static void setCurvatures(std::vector<Waypoint>& ipath);

  /**
   * Sets the waypoint velocities respecting curvature and deceleration. Traverses the path
   * backwards using a rate limiter.
   *
   * @param ipath  The path
   * @param limits The pure pursuit limits
   */
  static void setVelocity(std::vector<Waypoint>& ipath, const PursuitLimits& limits);

  /**
   * Gets the curvature of a given segment.
   *
   * @param  prev  The previous point
   * @param  point The current point
   * @param  next  The next point
   * @return The curvature
   */
  static double calculateCurvature(const Vector& prev, const Vector& point, const Vector& next);
};
} // namespace lib7842
