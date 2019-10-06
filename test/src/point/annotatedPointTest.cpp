#include "test.hpp"
#include "lib7842/point/annotatedPoint.hpp"

class AnnotatedPointTest : public ::testing::Test {
 protected:
  AnnotatedPoint point {5_in, 2_in};
};

TEST_F(AnnotatedPointTest, Constructors) {
  AnnotatedPoint();
  AnnotatedPoint(5_in, 2_in);
  AnnotatedPoint pointCopy(point);
}

TEST_F(AnnotatedPointTest, SetAndReadData) {
  point.setData("curvature", 5_curv);
  point.setData("distance", 5_m);
  point.setData("velocity", 5_mps);
  point.setData("segmentIndex", 5);

  EXPECT_EQ(point.getData<QCurvature>("curvature"), 5_curv);
  EXPECT_EQ(point.getData<QLength>("distance"), 5_m);
  EXPECT_EQ(point.getData<QSpeed>("velocity"), 5_mps);
  EXPECT_EQ(point.getData<double>("segmentIndex"), 5);
}

TEST_F(AnnotatedPointTest, TypeErrors) {
  point.setData("curvature", 5_m);
  point.setData("distance", 5_curv);
  point.setData("velocity", 5);
  point.setData("segmentIndex", 5_mps);

  ASSERT_THROW(point.getData<QCurvature>("curvature"), std::runtime_error);
  ASSERT_THROW(point.getData<QLength>("distance"), std::runtime_error);
  ASSERT_THROW(point.getData<QSpeed>("velocity"), std::runtime_error);
  ASSERT_THROW(point.getData<double>("segmentIndex"), std::runtime_error);
}

TEST_F(AnnotatedPointTest, NoTypeNoThrow) {
  ASSERT_THROW(point.getData<QCurvature>("curvature"), std::runtime_error);
  ASSERT_THROW(point.getData<QLength>("distance"), std::runtime_error);
  ASSERT_THROW(point.getData<QSpeed>("velocity"), std::runtime_error);
  ASSERT_THROW(point.getData<double>("segmentIndex"), std::runtime_error);
}