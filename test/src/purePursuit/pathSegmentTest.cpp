#include "test.hpp"
#include "lib7842/purePursuit/pathSegment.hpp"

class PathSegmentTest : public ::testing::Test {
 protected:
  PathSegment segment;
  PathPoint point1 {5_in, 3_in};
};

TEST_F(PathSegmentTest, Constructors) {
  PathSegment();
  PathSegment({});
  PathSegment({point1});
  PathSegment({{point1, point1}});
}

TEST_F(PathSegmentTest, AddSegments) {
  segment.addSegment(point1);
  segment.addSegments({point1, point1});
  segment.addSegment(PathSegment({point1}));
}

TEST_F(PathSegmentTest, ExtractSegments) {
  segment.addSegment(point1);
  segment.addSegments({point1, point1});
  segment.addSegment(PathSegment({point1, point1}));
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 5);
  for (auto&& point : path) {
    ASSERT_EQ(point, point1);
  }
}

TEST_F(PathSegmentTest, ProperOrder) {
  segment.addSegment(PathPoint {1_in, 2_in});
  segment.addSegments({PathPoint {2_in, 3_in}, PathPoint {3_in, 4_in}});
  segment.addSegment(PathSegment({PathPoint {4_in, 5_in}, PathPoint {5_in, 6_in}}));
  segment.addSegment(
    {PathSegment({PathSegment(PathPoint {6_in, 7_in}), PathSegment(PathPoint {7_in, 8_in})})});
  segment.addSegment(PathSegment({PathPoint(8_in, 9_in)}));
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 8);
  for (size_t i = 0; i < path.size(); ++i) {
    ASSERT_EQ(path[i], (PathPoint {(i + 1) * inch, (i + 2) * inch}));
  }
}