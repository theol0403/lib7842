#include "test.hpp"
#include "lib7842/path/packedPath.hpp"

class PackedPathTest : public ::testing::Test {
 protected:
  PackedPoint point1 {5_in, 3_in};
  void SetUp() override {
    point1.setData("curvature", 5_curv);
    point1.setData("distance", 5_m);
    point1.setData("velocity", 5_mps);
    point1.setData("segmentIndex", 5);
  }
};

TEST_F(PackedPathTest, Constructors) {
  PackedPath();
  PackedPath({point1});
  PackedPath({point1, point1});
  PackedPath(std::vector<PackedPoint>({point1, point1}));
}

TEST_F(PackedPathTest, ExtractReference) {
  SimplePath path({point1, point1});

  ReferencePath ipath = path.extractRef();
  ASSERT_EQ(ipath.get().size(), 2);
  for (auto&& point : ipath.get()) {
    ASSERT_EQ(point.get(), point1);
  }

  ReferencePath ipath2 = path.extractRef();
  for (size_t i = 0; i < ipath2.get().size(); i++) {
    ASSERT_EQ(&ipath.get()[i].get(), &ipath2.get()[i].get());
    ASSERT_EQ(&ipath.get()[i].get(), &path.get()[i]);
  }
}

TEST_F(PackedPathTest, ExtractPath) {
  PackedPath path({point1, point1, point1});

  SimplePath ipath = path.extract();
  ASSERT_EQ(ipath.get().size(), 3);
  for (auto&& point : ipath.get()) {
    ASSERT_EQ(point, point1);
  }
}

TEST_F(PackedPathTest, ExtractData) {
  PackedPath path({point1, point1, point1});

  ASSERT_EQ(path.get().size(), 3);
  for (auto&& point : path.get()) {
    EXPECT_EQ(point.getData<QCurvature>("curvature"), 5_curv);
    EXPECT_EQ(point.getData<QLength>("distance"), 5_m);
    EXPECT_EQ(point.getData<QSpeed>("velocity"), 5_mps);
    EXPECT_EQ(point.getData<int>("segmentIndex"), 5);
  }
}

TEST_F(PackedPathTest, ExtractReferenceData) {
  PackedPath path({point1, point1, point1});

  ReferencePath ipath = path.extractRef();

  ASSERT_EQ(ipath.get().size(), 3);
  for (auto&& point : ipath.get()) {
    const PackedPoint* iptr = dynamic_cast<const PackedPoint*>(&point.get());
    EXPECT_EQ(iptr->getData<QCurvature>("curvature"), 5_curv);
    EXPECT_EQ(iptr->getData<QLength>("distance"), 5_m);
    EXPECT_EQ(iptr->getData<QSpeed>("velocity"), 5_mps);
    EXPECT_EQ(iptr->getData<int>("segmentIndex"), 5);
  }
}
