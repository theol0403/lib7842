#include "test.hpp"
#include "lib7842/path/simplePath.hpp"

class SimplePathTest : public ::testing::Test {
protected:
  Vector point1 {5_in, 3_in};
};

TEST_F(SimplePathTest, SimpleTest) {
  SimplePath path({point1});
  SimplePath path2 = path.copy();
}

TEST_F(SimplePathTest, Constructors) {
  SimplePath();
  SimplePath({point1});
  SimplePath({point1, point1});
}

TEST_F(SimplePathTest, ExtractReference) {
  SimplePath path({point1, point1});

  ReferencePath ipath = path.ref();
  ASSERT_EQ(ipath().size(), 2);
  for (auto&& point : ipath()) {
    ASSERT_EQ(point.get(), point1);
  }

  ReferencePath ipath2 = path.ref();
  for (size_t i = 0; i < ipath2().size(); i++) {
    ASSERT_EQ(&ipath()[i].get(), &ipath2()[i].get());
    ASSERT_EQ(&ipath()[i].get(), path()[i].get());
  }
}

TEST_F(SimplePathTest, ExtractPath) {
  SimplePath path({point1, point1, point1});

  SimplePath ipath = path.extract();
  ASSERT_EQ(ipath().size(), 3);
  for (auto&& point : ipath()) {
    ASSERT_EQ(*point, point1);
  }
}

TEST_F(SimplePathTest, GenerateInsertPoints) {
  SimplePath path = SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}}).generate(20);
  ASSERT_EQ(path().size(), 20);
}

// TEST_F(SimplePathTest, GenerateSmoothPointsDontCopy) {
//   SimplePath path =
//     LinearGenerator::smoothen(SimplePath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}}), 0.25, 0.0001_in);
//   ASSERT_NE(std::round(path()[1].x.convert(inch) * 10) / 10, 2.5);
// }

// TEST_F(SimplePathTest, GenerateSmoothPointsDontPassWayPoints) {
//   SimplePath ipath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}});
//   SimplePath path = LinearGenerator::smoothen(ipath, 0.25, 0.0001_in);
//   for (size_t i = 1; i < path().size() - 1; ++i) {
//     ASSERT_NE(path()[i], ipath()[i]);
//   }
// }

// TEST_F(SimplePathTest, GenerateSmoothPointsFunction) {
//   SimplePath path = LinearGenerator::generate(
//     SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}), 0.5_in, 0.25, 0.0001_in);

//   ASSERT_NEAR(path()[10].x.convert(inch), 5, 0.1);
//   ASSERT_NEAR(path()[10].y.convert(inch), 3.5, 0.3);
// }
