#include "test.hpp"

class MockPathGenerator : public PathGenerator {
public:
  using PathGenerator::PathGenerator;
  using PathGenerator::calculateCurvature;
  using PathGenerator::setCurvatures;
  using PathGenerator::setMaxVelocity;
};

TEST_CASE("PathGenerator test") {
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
    PursuitPath pathStraight({{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}});
    MockPathGenerator::setCurvatures(pathStraight);

    CHECK(pathStraight()[1]->getData<double>("curvature") == 0);

    CHECK(pathStraight()[0]->getData<double>("curvature") == 0);
    CHECK(pathStraight()[2]->getData<double>("curvature") == 0);

    PursuitPath pathCurv({{0_m, 0_m}, {3_m, 5_m}, {0_m, 10_m}});
    MockPathGenerator::setCurvatures(pathCurv);
    CHECK(pathCurv()[1]->getData<double>("curvature") != 0);

    CHECK(pathCurv()[0]->getData<double>("curvature") == 0);
    CHECK(pathCurv()[2]->getData<double>("curvature") == 0);

    PursuitPath pathTurn({{0_m, 0_m}, {3_m, 5_m}, {0_m, 0_m}});
    MockPathGenerator::setCurvatures(pathTurn);
    CHECK(pathTurn()[1]->getData<double>("curvature") == 0);

    CHECK(pathTurn()[0]->getData<double>("curvature") == 0);
    CHECK(pathTurn()[2]->getData<double>("curvature") == 0);
  }

  SUBCASE("SetMaxVelocity") {
    PursuitPath path({{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}});
    MockPathGenerator::setCurvatures(path);
    MockPathGenerator::setMaxVelocity(path, limits);

    CHECK(path()[0]->getData<QSpeed>("velocity") == 8_mps);
    CHECK(path()[1]->getData<QSpeed>("velocity") == 8_mps);
    CHECK(path()[2]->getData<QSpeed>("velocity") == 3_mps);
  }

  SUBCASE("SetMaxVelocityTurn") {
    PursuitPath path({{0_m, 0_m}, {3_m, 4_m}, {6_m, 10_m}, {5_m, 12_m}});
    MockPathGenerator::setCurvatures(path);
    MockPathGenerator::setMaxVelocity(path, limits);

    CHECK(path()[0]->getData<QSpeed>("velocity") == 8_mps);
    CHECK(path()[1]->getData<QSpeed>("velocity") < 8_mps);
    CHECK(path()[2]->getData<QSpeed>("velocity") < 8_mps);
    CHECK(path()[3]->getData<QSpeed>("velocity") == 3_mps);
  }
}