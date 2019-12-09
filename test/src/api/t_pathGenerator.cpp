// #include "test.hpp"

// class PathGeneratorTest : public ::testing::Test, public PathGenerator {
// protected:
//   PursuitLimits limits {2_mps, 8_mps, 8_mps2, 0.03_mps};
// };

// TEST_F(PathGeneratorTest, ComputeSingleCurvature) {
//   double straight = PathGenerator::calculateCurvature({0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m});
//   ASSERT_EQ(straight, 0);

//   double curvature = PathGenerator::calculateCurvature({0_m, 0_m}, {3_m, 5_m}, {0_m, 10_m});
//   ASSERT_NE(curvature, 0);

//   double turn = PathGenerator::calculateCurvature({0_m, 0_m}, {3_m, 5_m}, {0_m, 0_m});
//   ASSERT_EQ(turn, 0);
// }

// TEST_F(PathGeneratorTest, SetCurvatures) {
//   PursuitPath pathStraight({{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}});
//   PathGenerator::setCurvatures(pathStraight);

//   ASSERT_EQ(pathStraight()[1]->getData<double>("curvature"), 0);

//   ASSERT_EQ(pathStraight()[0]->getData<double>("curvature"), 0);
//   ASSERT_EQ(pathStraight()[2]->getData<double>("curvature"), 0);

//   PursuitPath pathCurv({{0_m, 0_m}, {3_m, 5_m}, {0_m, 10_m}});
//   PathGenerator::setCurvatures(pathCurv);
//   ASSERT_NE(pathCurv()[1]->getData<double>("curvature"), 0);

//   ASSERT_EQ(pathCurv()[0]->getData<double>("curvature"), 0);
//   ASSERT_EQ(pathCurv()[2]->getData<double>("curvature"), 0);

//   PursuitPath pathTurn({{0_m, 0_m}, {3_m, 5_m}, {0_m, 0_m}});
//   PathGenerator::setCurvatures(pathTurn);
//   ASSERT_EQ(pathTurn()[1]->getData<double>("curvature"), 0);

//   ASSERT_EQ(pathTurn()[0]->getData<double>("curvature"), 0);
//   ASSERT_EQ(pathTurn()[2]->getData<double>("curvature"), 0);
// }

// TEST_F(PathGeneratorTest, SetMaxVelocity) {
//   PursuitPath path({{0_m, 0_m}, {0_m, 5_m}, {0_m, 10_m}});
//   PathGenerator::setCurvatures(path);
//   PathGenerator::setMaxVelocity(path, limits);

//   ASSERT_EQ(path()[0]->getData<QSpeed>("velocity").convert(mps), 8);
//   ASSERT_EQ(path()[1]->getData<QSpeed>("velocity").convert(mps), 8);
//   ASSERT_EQ(path()[2]->getData<QSpeed>("velocity").convert(mps), 0);
// }

// TEST_F(PathGeneratorTest, SetMaxVelocityTurn) {
//   PursuitPath path({{0_m, 0_m}, {3_m, 4_m}, {6_m, 10_m}, {5_m, 12_m}});
//   PathGenerator::setCurvatures(path);
//   PathGenerator::setMaxVelocity(path, limits);

//   ASSERT_EQ(path()[0]->getData<QSpeed>("velocity").convert(mps), 8);
//   ASSERT_LT(path()[1]->getData<QSpeed>("velocity").convert(mps), 8);
//   ASSERT_LT(path()[2]->getData<QSpeed>("velocity").convert(mps), 8);
//   ASSERT_EQ(path()[3]->getData<QSpeed>("velocity").convert(mps), 0);
// }