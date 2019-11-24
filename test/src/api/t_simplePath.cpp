#include "test.hpp"

class SimplePathTest : public ::testing::Test {
protected:
  Vector point1 {5_in, 3_in};
};

TEST_F(SimplePathTest, Constructors) {
  SimplePath();
  SimplePath({point1});
  SimplePath({point1, point1});
  SimplePath(std::vector<Vector>({point1, point1}));
}

TEST_F(SimplePathTest, ExtractPointer) {
  SimplePath path({point1, point1});

  // measure values
  SimplePath ipath = path;
  ASSERT_EQ(ipath().size(), 2);
  for (auto&& point : ipath()) {
    ASSERT_EQ(*point, point1);
  }

  // make sure pointers are equal
  SimplePath ipath2 = path;
  for (size_t i = 0; i < ipath().size(); i++) {
    ASSERT_EQ(ipath()[i], ipath2()[i]);
    ASSERT_EQ(ipath()[i], path()[i]);
  }
}

TEST_F(SimplePathTest, ExtractCopy) {
  SimplePath path({point1, point1});

  SimplePath ipath = path.copy();
  ASSERT_EQ(ipath().size(), 2);

  for (size_t i = 0; i < path().size(); i++) {
    // pointers should not be equal
    ASSERT_NE(path()[i], ipath()[i]);
    // values should be equal
    ASSERT_EQ(*path()[i], point1);
  }
}

TEST_F(SimplePathTest, GenerateExtractPath) {
  SimplePath path({point1, point1, point1});

  // generate with no interpolation
  SimplePath ipath = path.generate();
  ASSERT_EQ(ipath().size(), 3);
  for (auto&& point : ipath()) {
    ASSERT_EQ(*point, point1);
  }
}

TEST_F(SimplePathTest, GenerateInvalid) {
  SimplePath path({point1, point1, point1});

  EXPECT_THROW(path.generate(0), std::runtime_error);
  EXPECT_THROW(path.generate(0.1), std::runtime_error);
}

TEST_F(SimplePathTest, GenerateEmptyPath) {
  SimplePath path;

  // extract empty path
  ASSERT_EQ(path.generate(1)().size(), 0);
  ASSERT_EQ(path.generate(5)().size(), 0);
  EXPECT_THROW(path.generate(0), std::runtime_error);
}

TEST_F(SimplePathTest, GenerateSinglePointPath) {
  SimplePath path({point1});
  SimplePath ipath = path.generate();
  ASSERT_EQ(ipath().size(), 1);
  ASSERT_EQ(*ipath()[0], point1);
}

TEST_F(SimplePathTest, GenerateCorrectAmount) {
  SimplePath path({point1, point1, point1});

  SimplePath ipath2 = path.generate(2);
  ASSERT_EQ(ipath2().size(), 5);
  for (auto&& point : ipath2()) {
    ASSERT_EQ(*point, point1);
  }

  SimplePath ipath3 = path.generate(5);
  ASSERT_EQ(ipath3().size(), 11);
  for (auto&& point : ipath3()) {
    ASSERT_EQ(*point, point1);
  }
}

TEST_F(SimplePathTest, GenerateInsertPoints) {
  SimplePath path = SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}, {10_in, 3_in}}).generate(10);
  ASSERT_EQ(path().size(), 31);
}

TEST_F(SimplePathTest, GenerateSmoothPointsDontCopy) {
  SimplePath path({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}});
  path.smoothen(0.25, 0.0001_in);
  ASSERT_NE(std::round(path()[1]->x.convert(inch) * 10) / 10, 2.5);
}

TEST_F(SimplePathTest, GenerateSmoothPointsDontPassWayPoints) {
  SimplePath ipath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}});
  SimplePath path = ipath.copy();
  path.smoothen(0.25, 0.0001_in);
  for (size_t i = 1; i < path().size() - 1; i++) {
    ASSERT_NE(*path()[i], *ipath()[i]);
  }
}

TEST_F(SimplePathTest, GenerateSmoothPointsFunction) {
  SimplePath path = SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}).generate(10);
  ASSERT_EQ(path().size(), 21);
  path.smoothen(0.25, 0.0001_in);

  ASSERT_NEAR(path()[10]->x.convert(inch), 5, 0.1);
  ASSERT_NEAR(path()[10]->y.convert(inch), 3.5, 0.3);
}
