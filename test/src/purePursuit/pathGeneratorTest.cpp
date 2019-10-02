#include "test.hpp"
#include "lib7842/purePursuit/pathGenerator.hpp"

class PathGeneratorTest : public ::testing::Test {
 protected:
  PathSegment segment =
    PathSegment().addPoint({1_in, 1_in}).addPoint({5_in, 4_in}).addPoint({9_in, 2_in});
};

class PathGeneratorTester : public PathGenerator {
 public:
  using PathGenerator::PathGenerator;
  using PathGenerator::path;
};

TEST_F(PathGeneratorTest, InsertPoints) {
  PathGeneratorTester generator(segment);
  generator.insertPoints(0.5_in);
  ASSERT_EQ(generator.path.size(), 20);
}