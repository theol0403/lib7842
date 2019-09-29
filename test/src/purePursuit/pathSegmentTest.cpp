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
  PathSegment point2(point1);
  PathSegment({point1});
  PathSegment({{point1, point1}});
}

TEST_F(PathSegmentTest, AddSegments) {
  segment.addPoint(point1);
  segment.addPoints({point1, point1});
  segment.addSegment({point1});
}

TEST_F(PathSegmentTest, ExtractSegments) {
  segment.addPoint(point1);
  segment.addPoints({point1, point1});
  segment.addSegment(PathSegment({point1, point1}));
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 5);
  for (auto&& point : path) {
    ASSERT_EQ(point, point1);
  }
}

TEST_F(PathSegmentTest, ProperOrder) {
  segment.addPoint({1_in, 2_in});
  segment.addPoints({{2_in, 3_in}, {3_in, 4_in}});
  segment.addSegment(PathSegment({PathPoint {4_in, 5_in}, PathPoint {5_in, 6_in}}));
  segment.addSegments(
    {PathSegment({PathSegment(PathPoint {6_in, 7_in}), PathSegment(PathPoint {7_in, 8_in})}),
     PathSegment({PathPoint(8_in, 9_in)})});
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 8);
  for (size_t i = 0; i < path.size(); ++i) {
    ASSERT_EQ(path[i], (PathPoint {(i + 1) * inch, (i + 2) * inch}));
  }
}