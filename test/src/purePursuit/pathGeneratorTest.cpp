// #include "test.hpp"
// #include "lib7842/purePursuit/pathGenerator.hpp"

// class PathGeneratorTest : public ::testing::Test {
//  protected:
// };

// class PathGeneratorTester : public PathGenerator {
//  public:
//   using PathGenerator::PathGenerator;
//   using PathGenerator::path;
// };

// TEST_F(PathGeneratorTest, InsertPoints) {
//   PathGeneratorTester generator(
//     PathSegment().addPoints({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}}));

//   generator.insertPoints(0.5_in);
//   ASSERT_EQ(generator.path.size(), 20);
// }

// TEST_F(PathGeneratorTest, SmoothPointsCopy) {
//   PathGeneratorTester generator(
//     PathSegment().addPoints({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}}));

//   generator.smoothen({0.25, 0.0001_in});

//   ASSERT_NE(std::round(generator.path[1].x.convert(inch) * 10) / 10, 2.5);
// }

// TEST_F(PathGeneratorTest, SmoothPointsDontPassMiddlePoints) {
//   std::vector<Vector> path =
//     PathSegment()
//       .addPoints({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}})
//       .extract();

//   PathGeneratorTester generator(PathSegment().addPoints(path));
//   generator.smoothen({0.25, 0.0001_in});

//   for (size_t i = 1; i < generator.path.size() - 1; ++i) {
//     ASSERT_NE(generator.path[i], path[i]);
//   }
// }

// TEST_F(PathGeneratorTest, SmoothPointsWork) {
//   PathGeneratorTester generator(
//     PathSegment().addPoints({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}));

//   generator.insertPoints(0.5_in);
//   generator.smoothen({0.25, 0.0001_in});

//   ASSERT_NEAR(generator.path[10].x.convert(inch), 5, 0.1);
//   ASSERT_NEAR(generator.path[10].y.convert(inch), 3.5, 0.3);
// }

// TEST_F(PathGeneratorTest, ComputeDistances) {
//   PathGeneratorTester generator(PathSegment().addPoints({{0_in, 0_in}, {20_in, 0_in}}));

//   generator.insertPoints(1_in);
//   generator.computeDistances();

//   for (size_t i = 0; i < generator.path.size(); i++) {
//     ASSERT_NEAR(generator.path[i].getValue<QLength>("distance").convert(inch), i, 1e-10);
//   }
// }

// TEST_F(PathGeneratorTest, ComputeSingleCurvature) {
//   QCurvature straight =
//     PathGenerator::computeSingleCurvature({0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in});
//   ASSERT_EQ(straight, 0_curv);

//   QCurvature curv =
//     PathGenerator::computeSingleCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in});
//   ASSERT_NE(curv, 0_curv);

//   QCurvature turn = PathGenerator::computeSingleCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in});
//   ASSERT_EQ(turn, 0_curv);
// }

// TEST_F(PathGeneratorTest, ComputeCurvatures) {
//   PathGeneratorTester generatorStraight(
//     PathSegment().addPoints({{0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in}}));
//   generatorStraight.computeCurvatures();
//   ASSERT_EQ(generatorStraight.path[1].getValue<QCurvature>("curvature"), 0_curv);

//   ASSERT_EQ(generatorStraight.path[0].getValue<QCurvature>("curvature"), 0_curv);
//   ASSERT_EQ(generatorStraight.path[2].getValue<QCurvature>("curvature"), 0_curv);

//   PathGeneratorTester generatorCurv(
//     PathSegment().addPoints({{0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}}));
//   generatorCurv.computeCurvatures();
//   ASSERT_NE(generatorCurv.path[1].getValue<QCurvature>("curvature"), 0_curv);

//   ASSERT_EQ(generatorCurv.path[0].getValue<QCurvature>("curvature"), 0_curv);
//   ASSERT_EQ(generatorCurv.path[2].getValue<QCurvature>("curvature"), 0_curv);

//   PathGeneratorTester generatorTurn(
//     PathSegment().addPoints({{0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in}}));
//   generatorTurn.computeCurvatures();
//   ASSERT_EQ(generatorTurn.path[1].getValue<QCurvature>("curvature"), 0_curv);

//   ASSERT_EQ(generatorTurn.path[0].getValue<QCurvature>("curvature"), 0_curv);
//   ASSERT_EQ(generatorTurn.path[2].getValue<QCurvature>("curvature"), 0_curv);
// }