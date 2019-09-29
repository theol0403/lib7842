#include "test.hpp"
#include "lib7842/purePursuit/pathPoint.hpp"

class PathPointTest : public ::testing::Test {
  protected:
  PathPoint point {5_in, 2_in};
};

TEST_F(PathPointTest, SetData) {
  point.setData("curvature", 5);
  point.setData("distance", 5_m);
  point.setData("velocity", 5_mps);
  point.setData("segmentIndex", 5);
}

TEST_F(PathPointTest, ReadData) {
  point.setData("curvature", 5);
  point.setData("distance", 5_m);
  point.setData("velocity", 5_mps);
  point.setData("segmentIndex", 5);

  EXPECT_EQ(point.getCurvature(), 5);
  EXPECT_EQ(point.getDistance(), 5_m);
  EXPECT_EQ(point.getVelocity(), 5_mps);
  EXPECT_EQ(point.getSegmentIndex(), 5);
}

TEST_F(PathPointTest, TypeErrors) {
  point.setData("curvature", 5_m);
  point.setData("distance", 5);
  point.setData("velocity", 5);
  point.setData("segmentIndex", 5_mps);

  EXPECT_EQ(point.getCurvature(), 0);
  EXPECT_EQ(point.getDistance(), 0_m);
  EXPECT_EQ(point.getVelocity(), 0_mps);
  EXPECT_EQ(point.getSegmentIndex(), 0);
}

TEST_F(PathPointTest, IdErrors) {
  testing::internal::CaptureStderr();
  point.getCurvature();
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_EQ(output, "PathPoint::getData: invalid ID \"curvature\"\n");
}

TEST_F(PathPointTest, Constructors) {
  PathPoint();
  PathPoint(5_in, 2_in);
  PathPoint pointCopy(point);
}