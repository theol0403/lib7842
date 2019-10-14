#include "test.hpp"
#include "lib7842/purePursuit/pathPacker.hpp"
#include "lib7842/purePursuit/linearGenerator.hpp"

class PathPackerTest : public ::testing::Test {
protected:
};

TEST_F(PathPackerTest, SetDistancesSimple) {
  PackedPath path(SimplePath({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setDistances(path);

  ASSERT_EQ(path()[0].getData<QLength>("distance"), 0_in);
  ASSERT_EQ(path()[1].getData<QLength>("distance"), 5_in);
  ASSERT_EQ(path()[2].getData<QLength>("distance"), 10_in);
}

TEST_F(PathPackerTest, SetDistancesComplex) {
  PackedPath path(LinearGenerator::insert(SimplePath({{0_in, 0_in}, {0_in, 20_in}}), 1_in));
  PathPacker::setDistances(path);

  for (size_t i = 0; i < path().size(); i++) {
    ASSERT_NEAR(path()[i].getData<QLength>("distance").convert(inch), i, 1e-8);
  }
}

TEST_F(PathPackerTest, ComputeSingleCurvature) {
  QCurvature straight = PathPacker::getCurvature({0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in});
  ASSERT_EQ(straight, 0_curv);

  QCurvature curv = PathPacker::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in});
  ASSERT_NE(curv, 0_curv);

  QCurvature turn = PathPacker::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in});
  ASSERT_EQ(turn, 0_curv);
}

TEST_F(PathPackerTest, SetCurvatures) {
  PackedPath pathStraight(SimplePath({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setCurvatures(pathStraight);

  ASSERT_EQ(pathStraight()[1].getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathStraight()[0].getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathStraight()[2].getData<QCurvature>("curvature").convert(curvature), 0);

  PackedPath pathCurv(SimplePath({{0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setCurvatures(pathCurv);
  ASSERT_NE(pathCurv()[1].getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathCurv()[0].getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathCurv()[2].getData<QCurvature>("curvature").convert(curvature), 0);

  PackedPath pathTurn(SimplePath({{0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in}}));
  PathPacker::setCurvatures(pathTurn);
  ASSERT_EQ(pathTurn()[1].getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathTurn()[0].getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathTurn()[2].getData<QCurvature>("curvature").convert(curvature), 0);
}

TEST_F(PathPackerTest, SetMaxVelocity) {
  PackedPath path(SimplePath({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setCurvatures(path);
  PathPacker::setMaxVelocity(path, {0_mps, 0.3_mps, 1_mps2, 100_curv});

  ASSERT_EQ(path()[0].getData<QSpeed>("velocity").convert(mps), 0.3);
  ASSERT_EQ(path()[1].getData<QSpeed>("velocity").convert(mps), 0.3);
  ASSERT_EQ(path()[2].getData<QSpeed>("velocity").convert(mps), 0);
}

TEST_F(PathPackerTest, SetMaxVelocityTurn) {
  PackedPath path(SimplePath({{0_m, 0_m}, {3_m, 4_m}, {6_m, 10_m}, {3_m, 15_m}}));
  PathPacker::setCurvatures(path);
  PathPacker::setMaxVelocity(path, {0_mps, 8_mps, 5_mps2, 20_curv});

  ASSERT_EQ(path()[0].getData<QSpeed>("velocity").convert(mps), 8);
  ASSERT_EQ(path()[1].getData<QSpeed>("velocity").convert(mps), 8);
  ASSERT_LT(path()[2].getData<QSpeed>("velocity").convert(mps), 8);
  ASSERT_EQ(path()[3].getData<QSpeed>("velocity").convert(mps), 0);
}