#include "test.hpp"

class DataPointTest : public ::testing::Test {
protected:
  DataPoint point {5_in, 2_in};
};

TEST_F(DataPointTest, Constructors) {
  DataPoint();
  DataPoint(5_in, 2_in);
  DataPoint pointCopy(point);
}

TEST_F(DataPointTest, SetAndReadData) {
  point.setData("curvature", 5.0);
  point.setData("distance", 5_m);
  point.setData("velocity", 5_mps);
  point.setData("segmentIndex", 5);

  EXPECT_EQ(point.getData<double>("curvature"), 5.0);
  EXPECT_EQ(point.getData<QLength>("distance"), 5_m);
  EXPECT_EQ(point.getData<QSpeed>("velocity"), 5_mps);
  EXPECT_EQ(point.getData<int>("segmentIndex"), 5);
}

TEST_F(DataPointTest, TypeErrors) {
  point.setData("curvature", 5_m);
  point.setData("distance", 5);
  point.setData("velocity", 5);
  point.setData("segmentIndex", 5_mps);

  ASSERT_THROW(point.getData<double>("curvature"), std::runtime_error);
  ASSERT_THROW(point.getData<QLength>("distance"), std::runtime_error);
  ASSERT_THROW(point.getData<QSpeed>("velocity"), std::runtime_error);
  ASSERT_THROW(point.getData<int>("segmentIndex"), std::runtime_error);
}

TEST_F(DataPointTest, NoID) {
  ASSERT_THROW(point.getData<double>("curvature"), std::runtime_error);
  ASSERT_THROW(point.getData<QLength>("distance"), std::runtime_error);
  ASSERT_THROW(point.getData<QSpeed>("velocity"), std::runtime_error);
  ASSERT_THROW(point.getData<int>("segmentIndex"), std::runtime_error);
}