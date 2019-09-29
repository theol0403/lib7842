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
}

TEST_F(PathSegmentTest, AddSegments) {
  segment.addPoint(point1);
  segment.addPoints({point1, point1});
  segment.addSegment(PathSegment {});
}

TEST_F(PathSegmentTest, ExtractSegments) {
  segment.addPoint(point1);
  segment.addPoints({point1, point1});
  segment.addSegment(PathSegment());
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 3);
  for (auto&& point : path) {
    ASSERT_EQ(point, point1);
  }
}

TEST_F(PathSegmentTest, ProperOrder) {
  segment.addPoint({1_in, 2_in});
  segment.addPoints({{2_in, 3_in}, {3_in, 4_in}});

  PathSegment segment1;
  segment1.addPoints({{4_in, 5_in}, {5_in, 6_in}});
  segment.addSegment(segment1);

  PathSegment segment2;
  segment2.addPoint({6_in, 7_in});

  PathSegment segment3;
  segment3.addPoint({7_in, 8_in});

  PathSegment segment4;
  segment4.addSegments({segment2, PathSegment(segment3)});

  PathSegment segment5;
  segment5.addPoint({8_in, 9_in});

  segment.addSegments({segment4, segment5});
  std::vector<PathPoint> path = segment.extract();

  ASSERT_EQ(path.size(), 8);
  for (size_t i = 0; i < path.size(); ++i) {
    ASSERT_EQ(path[i], (PathPoint {(i + 1) * inch, (i + 2) * inch}));
  }
}