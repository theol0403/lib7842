#include "test.hpp"
#include "lib7842/point/point.hpp"

class QPointTest : public ::testing::Test {
 protected:
  QPoint point {5_in, 2_in};
  QPoint emptyPoint;
};

TEST_F(QPointTest, Constructor) {
  EXPECT_EQ(point.x.convert(inch), 5);
  EXPECT_EQ(point.y.convert(inch), 2);
}

TEST_F(QPointTest, CopyConstructor) {
  EXPECT_EQ(QPoint(point), point);
}

TEST_F(QPointTest, DefaultInitialization) {
  EXPECT_EQ(emptyPoint, (QPoint {0_in, 0_in}));
}

TEST_F(QPointTest, MathOperators) {
  EXPECT_EQ((point + point).x, point.x * 2);
  EXPECT_EQ((point + point).y, point.y * 2);

  EXPECT_EQ((point - point), emptyPoint);
}

TEST_F(QPointTest, EqualityOperators) {
  EXPECT_EQ(point, point);
  EXPECT_EQ(point + (QPoint {1_in, 2_in}), point + (QPoint {1_in, 2_in}));

  ASSERT_NE(point, point + point);
}

TEST_F(QPointTest, AccessorOperator) {
  EXPECT_EQ(point[0], point.x);
  EXPECT_EQ(point[1], point.y);
  ASSERT_THROW(point[2], std::runtime_error);
  ASSERT_THROW(point[5], std::runtime_error);
  ASSERT_THROW(point[-1], std::runtime_error);
}
