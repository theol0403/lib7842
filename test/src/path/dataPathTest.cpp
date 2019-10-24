#include "test.hpp"
#include "lib7842/path/dataPath.hpp"

class DataPathTest : public ::testing::Test {
protected:
  DataPoint point1 {5_in, 3_in};
  void SetUp() override {
    point1.setData("curvature", 5_curv);
    point1.setData("distance", 5_m);
    point1.setData("velocity", 5_mps);
    point1.setData("segmentIndex", 5);
  }
};

TEST_F(DataPathTest, Constructors) {
  DataPath();
  DataPath({point1});
  DataPath({point1, point1});
  DataPath(std::vector<DataPoint>({point1, point1}));
}

TEST_F(DataPathTest, ExtractData) {
  DataPath path({point1, point1, point1});

  ASSERT_EQ(path().size(), 3);
  for (auto&& point : path()) {
    EXPECT_EQ(point->getData<QCurvature>("curvature"), 5_curv);
    EXPECT_EQ(point->getData<QLength>("distance"), 5_m);
    EXPECT_EQ(point->getData<QSpeed>("velocity"), 5_mps);
    EXPECT_EQ(point->getData<int>("segmentIndex"), 5);
  }
}

TEST_F(DataPathTest, Generate) {
  DataPath path({point1, point1, point1});

  SimplePath ipath = path.generate();
  ASSERT_EQ(ipath().size(), 3);
  for (auto&& point : ipath()) {
    ASSERT_EQ(*point, point1);
  }
}
