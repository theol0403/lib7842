#include "test.hpp"
#include "lib7842/purePursuit/pathPoint.hpp"

class PathPointTest : public ::testing::Test {
 protected:
  PathPoint point {5_in, 2_in};
};

TEST_F(PathPointTest, Constructors) {
  PathPoint();
  PathPoint(5_in, 2_in);
  PathPoint pointCopy(point);
}

TEST_F(PathPointTest, SetAndReadData) {
  point.setData("curvature", 5_curv);
  point.setData("distance", 5_m);
  point.setData("velocity", 5_mps);
  point.setData("segmentIndex", 5);

  EXPECT_EQ(point.getValue<QCurvature>("curvature"), 5_curv);
  EXPECT_EQ(point.getValue<QLength>("distance"), 5_m);
  EXPECT_EQ(point.getValue<QSpeed>("velocity"), 5_mps);
  EXPECT_EQ(point.getValue<double>("segmentIndex"), 5);
}

TEST_F(PathPointTest, TypeErrors) {
  point.setData("curvature", 5_m);
  point.setData("distance", 5_curv);
  point.setData("velocity", 5);
  point.setData("segmentIndex", 5_mps);

  ASSERT_THROW(point.getValue<QCurvature>("curvature"), std::runtime_error);
  ASSERT_THROW(point.getValue<QLength>("distance"), std::runtime_error);
  ASSERT_THROW(point.getValue<QSpeed>("velocity"), std::runtime_error);
  ASSERT_THROW(point.getValue<double>("segmentIndex"), std::runtime_error);
}

TEST_F(PathPointTest, NoTypeNoThrow) {
  ASSERT_NO_THROW(point.getValue<QCurvature>("curvature"));
  ASSERT_NO_THROW(point.getValue<QLength>("distance"));
  ASSERT_NO_THROW(point.getValue<QSpeed>("velocity"));
  ASSERT_NO_THROW(point.getValue<double>("segmentIndex"));
}

TEST_F(PathPointTest, NoID) {
  EXPECT_EQ(point.getValue<QCurvature>("curvature"), 0_curv);
}