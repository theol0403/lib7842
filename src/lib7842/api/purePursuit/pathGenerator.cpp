#include "lib7842/api/purePursuit/pathGenerator.hpp"
#include "lib7842/api/positioning/path/line.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"

namespace lib7842 {

void PathGenerator::setCurvatures(std::vector<Waypoint>& ipath) {
  ipath.at(0).curvature = 0.0 / meter;
  for (size_t i = 1; i < ipath.size() - 1; i++) {
    double curvature = calculateCurvature(ipath.at(i - 1), ipath.at(i), ipath.at(i + 1));
    ipath[i].curvature = curvature / meter;
  }
  ipath.back().curvature = 0.0 / meter;
}

void PathGenerator::setVelocity(std::vector<Waypoint>& ipath, const PursuitLimits& limits) {
  ipath.back().velocity = limits.finalVel;
  for (size_t i = ipath.size() - 1; i > 0; i--) {
    auto& start = ipath[i];
    auto& end = ipath[i - 1];

    // k / curvature, limited to max
    QSpeed wantedVel =
      limits.k ? std::min(limits.maxVel, limits.k.value() / ipath[i].curvature.convert(1 / meter))
               : limits.maxVel;

    // distance from last point
    double distance = MathPoint::dist(start, end);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    QSpeed maxIncrement = mps * std::sqrt(std::pow(start.velocity.convert(mps), 2) +
                                          (2.0 * limits.decel.convert(mps2) * distance));

    // limiting to maximum accelerated velocity
    QSpeed newVel = std::min(wantedVel, maxIncrement);
    end.velocity = newVel;
  }
}

double PathGenerator::calculateCurvature(const Vector& prev, const Vector& point,
                                         const Vector& next) {
  double distOne = MathPoint::dist(point, prev);
  double distTwo = MathPoint::dist(point, next);
  double distThree = MathPoint::dist(next, prev);

  double productOfSides = distOne * distTwo * distThree;
  double semiPerimeter = (distOne + distTwo + distThree) / 2.0;

  double triangleArea = std::sqrt(semiPerimeter * //
                                  (semiPerimeter - distOne) * //
                                  (semiPerimeter - distTwo) * //
                                  (semiPerimeter - distThree));

  double r = productOfSides / (4.0 * triangleArea);
  double curvature = std::isnormal(1.0 / r) ? 1.0 / r : 0;
  return curvature * curvature;
}

} // namespace lib7842

#include "lib7842/test/test.hpp"
namespace test {
class MockPathGenerator : public PathGenerator {
public:
  using PathGenerator::PathGenerator;
  using PathGenerator::calculateCurvature;
  using PathGenerator::setCurvatures;
  using PathGenerator::setVelocity;
};

TEST_CASE("PathGenerator") {
  PursuitLimits limits {2_mps, 8_mps2, 8_mps, 8_mps2, 3_mps, 0.03_mps};

  SUBCASE("ComputeSingleCurvature") {
    double straight = MockPathGenerator::calculateCurvature({0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m});
    CHECK(straight == 0);

    double curvature = MockPathGenerator::calculateCurvature({0_m, 0_m}, {3_m, 5_m}, {0_m, 10_m});
    CHECK(curvature != 0);

    double turn = MockPathGenerator::calculateCurvature({0_m, 0_m}, {3_m, 5_m}, {0_m, 0_m});
    CHECK(turn == 0);
  }

  SUBCASE("SetCurvatures") {
    std::vector<Waypoint> pathStraight {{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}};
    MockPathGenerator::setCurvatures(pathStraight);

    CHECK(pathStraight[1].curvature == 0 / meter);

    CHECK(pathStraight[0].curvature == 0 / meter);
    CHECK(pathStraight[2].curvature == 0 / meter);

    std::vector<Waypoint> pathCurv({{0_m, 0_m}, {3_m, 5_m}, {0_m, 10_m}});
    MockPathGenerator::setCurvatures(pathCurv);
    CHECK(pathCurv[1].curvature != 0 / meter);

    CHECK(pathCurv[0].curvature == 0 / meter);
    CHECK(pathCurv[2].curvature == 0 / meter);

    std::vector<Waypoint> pathTurn({{0_m, 0_m}, {3_m, 5_m}, {0_m, 0_m}});
    MockPathGenerator::setCurvatures(pathTurn);
    CHECK(pathTurn[1].curvature == 0 / meter);

    CHECK(pathTurn[0].curvature == 0 / meter);
    CHECK(pathTurn[2].curvature == 0 / meter);
  }

  SUBCASE("SetMaxVelocity") {
    std::vector<Waypoint> path({{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}});
    MockPathGenerator::setCurvatures(path);
    MockPathGenerator::setVelocity(path, limits);

    CHECK(path[0].velocity == 8_mps);
    CHECK(path[1].velocity == 8_mps);
    CHECK(path[2].velocity == 3_mps);
  }

  SUBCASE("SetMaxVelocityTurn") {
    std::vector<Waypoint> path({{0_m, 0_m}, {3_m, 4_m}, {6_m, 10_m}, {5_m, 12_m}});
    MockPathGenerator::setCurvatures(path);
    MockPathGenerator::setVelocity(path, limits);

    CHECK(path[0].velocity == 8_mps);
    CHECK(path[1].velocity < 8_mps);
    CHECK(path[2].velocity < 8_mps);
    CHECK(path[3].velocity == 3_mps);
  }

  auto p = Line({{0_m, 0_m}, {0_m, 5_m}}).step(StepBy::T(0.01));
  PathGenerator::generate(p, limits);
}
} // namespace test
