#include "test.hpp"
#include "lib7842/purePursuit/pathPacker.hpp"

class PathPackerTest : public ::testing::Test {
protected:
};

TEST_F(PathPackerTest, ComputeSingleCurvature) {
  QCurvature straight = PathPacker::getCurvature({0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in});
  ASSERT_EQ(straight, 0_curv);

  QCurvature curv = PathPacker::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in});
  ASSERT_NE(curv, 0_curv);

  QCurvature turn = PathPacker::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in});
  ASSERT_EQ(turn, 0_curv);
}

TEST_F(PathPackerTest, ComputeCurvatures) {
  PackedPath pathStraight(SimplePath({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setCurvatures(pathStraight);

  ASSERT_EQ(pathStraight()[1].getData<QCurvature>("curvature"), 0_curv);

  ASSERT_EQ(pathStraight()[0].getData<QCurvature>("curvature"), 0_curv);
  ASSERT_EQ(pathStraight()[2].getData<QCurvature>("curvature"), 0_curv);

  PackedPath pathCurv(SimplePath({{0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}}));
  PathPacker::setCurvatures(pathCurv);
  ASSERT_NE(pathCurv()[1].getData<QCurvature>("curvature"), 0_curv);

  ASSERT_EQ(pathCurv()[0].getData<QCurvature>("curvature"), 0_curv);
  ASSERT_EQ(pathCurv()[2].getData<QCurvature>("curvature"), 0_curv);

  PackedPath pathTurn(SimplePath({{0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in}}));
  PathPacker::setCurvatures(pathTurn);
  ASSERT_EQ(pathTurn()[1].getData<QCurvature>("curvature"), 0_curv);

  ASSERT_EQ(pathTurn()[0].getData<QCurvature>("curvature"), 0_curv);
  ASSERT_EQ(pathTurn()[2].getData<QCurvature>("curvature"), 0_curv);
}