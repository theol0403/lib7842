#include "test.hpp"

class PathGeneratorTest : public ::testing::Test {
protected:
  PathGenerator::limits limits {2_ips, 8_ips, 8_ips2, 1_curv};
};

TEST_F(PathGeneratorTest, SetDistancesSimple) {
  DataPath path({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}});
  PathGenerator::setDistances(path);

  ASSERT_EQ(path()[0]->getData<QLength>("distance"), 0_in);
  ASSERT_EQ(path()[1]->getData<QLength>("distance"), 5_in);
  ASSERT_EQ(path()[2]->getData<QLength>("distance"), 10_in);
}

TEST_F(PathGeneratorTest, ComputeSingleCurvature) {
  QCurvature straight = PathGenerator::getCurvature({0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in});
  ASSERT_EQ(straight, 0_curv);

  QCurvature curv = PathGenerator::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in});
  ASSERT_NE(curv, 0_curv);

  QCurvature turn = PathGenerator::getCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in});
  ASSERT_EQ(turn, 0_curv);
}

TEST_F(PathGeneratorTest, SetCurvatures) {
  DataPath pathStraight({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}});
  PathGenerator::setCurvatures(pathStraight);

  ASSERT_EQ(pathStraight()[1]->getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathStraight()[0]->getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathStraight()[2]->getData<QCurvature>("curvature").convert(curvature), 0);

  DataPath pathCurv({{0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}});
  PathGenerator::setCurvatures(pathCurv);
  ASSERT_NE(pathCurv()[1]->getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathCurv()[0]->getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathCurv()[2]->getData<QCurvature>("curvature").convert(curvature), 0);

  DataPath pathTurn({{0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in}});
  PathGenerator::setCurvatures(pathTurn);
  ASSERT_EQ(pathTurn()[1]->getData<QCurvature>("curvature").convert(curvature), 0);

  ASSERT_EQ(pathTurn()[0]->getData<QCurvature>("curvature").convert(curvature), 0);
  ASSERT_EQ(pathTurn()[2]->getData<QCurvature>("curvature").convert(curvature), 0);
}

TEST_F(PathGeneratorTest, SetMaxVelocity) {
  DataPath path({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}});
  PathGenerator::setCurvatures(path);
  PathGenerator::setMaxVelocity(path, limits);

  ASSERT_EQ(path()[0]->getData<QSpeed>("velocity").convert(ips), 8);
  ASSERT_EQ(path()[1]->getData<QSpeed>("velocity").convert(ips), 8);
  ASSERT_EQ(path()[2]->getData<QSpeed>("velocity").convert(ips), 0);
}

TEST_F(PathGeneratorTest, SetMaxVelocityTurn) {
  DataPath path({{0_in, 0_in}, {3_in, 4_in}, {6_in, 10_in}, {5_in, 12_in}});
  PathGenerator::setCurvatures(path);
  PathGenerator::setMaxVelocity(path, limits);

  ASSERT_EQ(path()[0]->getData<QSpeed>("velocity").convert(ips), 8);
  ASSERT_LT(path()[1]->getData<QSpeed>("velocity").convert(ips), 8);
  ASSERT_LT(path()[2]->getData<QSpeed>("velocity").convert(ips), 8);
  ASSERT_EQ(path()[3]->getData<QSpeed>("velocity").convert(ips), 0);
}

TEST_F(PathGeneratorTest, SetMinVelocity) {
  DataPath path(SimplePath({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}).generate(10));
  PathGenerator::setCurvatures(path);
  PathGenerator::setMaxVelocity(path, limits);
  PathGenerator::setMinVelocity(path, limits);

  for (auto&& point : path()) {
    ASSERT_GE(point->getData<QSpeed>("velocity").convert(ips), 2);
  }
}