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
