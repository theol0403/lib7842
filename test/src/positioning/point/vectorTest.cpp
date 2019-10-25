#include "test.hpp"
#include "lib7842/positioning/point/vector.hpp"

class VectorTest : public ::testing::Test {
protected:
  Vector point {5_in, 2_in};
  Vector emptyPoint;
};

TEST_F(VectorTest, Constructor) {
  EXPECT_EQ(point.x.convert(inch), 5);
  EXPECT_EQ(point.y.convert(inch), 2);
}

TEST_F(VectorTest, CopyConstructor) {
  EXPECT_EQ(Vector(point), point);
}

TEST_F(VectorTest, DefaultInitialization) {
  EXPECT_EQ(emptyPoint, (Vector {0_in, 0_in}));
}

TEST_F(VectorTest, MathOperators) {
  EXPECT_EQ((point + point).x, point.x * 2);
  EXPECT_EQ((point + point).y, point.y * 2);

  EXPECT_EQ((point - point), emptyPoint);
}

TEST_F(VectorTest, EqualityOperators) {
  EXPECT_EQ(point, point);
  EXPECT_EQ(point + (Vector {1_in, 2_in}), point + (Vector {1_in, 2_in}));

  ASSERT_NE(point, point + point);
}

TEST_F(VectorTest, AccessorOperator) {
  EXPECT_EQ(point[0], point.x);
  EXPECT_EQ(point[1], point.y);
  ASSERT_THROW(point[2], std::runtime_error);
  ASSERT_THROW(point[5], std::runtime_error);
  ASSERT_THROW(point[-1], std::runtime_error);
}
