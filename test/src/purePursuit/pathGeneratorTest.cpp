#include "test.hpp"
#include "lib7842/purePursuit/pathGenerator.hpp"

class PathGeneratorTest : public ::testing::Test {
 protected:
};

class PathGeneratorTester : public PathGenerator {
 public:
  using PathGenerator::PathGenerator;
  using PathGenerator::path;
};

TEST_F(PathGeneratorTest, InsertPoints) {
  PathGeneratorTester generator(
    PathSegment().addPoints({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}}));

  generator.insertPoints(0.5_in);
  ASSERT_EQ(generator.path.size(), 20);
}

TEST_F(PathGeneratorTest, SmoothPointsCopy) {
  PathGeneratorTester generator(
    PathSegment().addPoints({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}}));

  generator.smoothen({0.25, 0.0001_in});

  ASSERT_NE(std::round(generator.path[1].x.convert(inch) * 10) / 10, 2.5);
}

TEST_F(PathGeneratorTest, SmoothPointsDontPassMiddlePoints) {
  std::vector<PathPoint> path =
    PathSegment()
      .addPoints({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}})
      .extract();

  PathGeneratorTester generator(PathSegment().addPoints(path));
  generator.smoothen({0.25, 0.0001_in});

  for (size_t i = 1; i < generator.path.size() - 1; ++i) {
    ASSERT_NE(generator.path[i], path[i]);
  }
}

TEST_F(PathGeneratorTest, SmoothPointsWork) {
  PathGeneratorTester generator(
    PathSegment().addPoints({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}));

  generator.insertPoints(0.5_in);
  generator.smoothen({0.25, 0.0001_in});

  ASSERT_NEAR(generator.path[10].x.convert(inch), 5, 0.1);
  ASSERT_NEAR(generator.path[10].y.convert(inch), 3.5, 0.3);
}

TEST_F(PathGeneratorTest, ComputeDistances) {
  PathGeneratorTester generator(PathSegment().addPoints({{0_in, 0_in}, {20_in, 0_in}}));

  generator.insertPoints(1_in);
  generator.computeDistances();

  for (size_t i = 0; i < generator.path.size(); i++) {
    ASSERT_NEAR(generator.path[i].getValue<QLength>("distance").convert(inch), i, 1e-10);
  }
}

TEST_F(PathGeneratorTest, ComputeSingleCurvature) {
  double straight =
    PathGenerator::computeSingleCurvature({0_in, 0_in}, {0_in, 5_in}, {0_in, 10_in});
  ASSERT_EQ(straight, 0);

  double curv = PathGenerator::computeSingleCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 10_in});
  ASSERT_NE(curv, 0);

  double turn = PathGenerator::computeSingleCurvature({0_in, 0_in}, {3_in, 5_in}, {0_in, 0_in});
  ASSERT_EQ(turn, 0);
}