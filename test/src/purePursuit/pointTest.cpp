#include "test.hpp"
#include "lib7842/purePursuit/point.hpp"

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

class QStateTest : public ::testing::Test {
 protected:
  QState state {5_in, 2_in, 3_rad};
  QPoint point {5_in, 2_in};
};

TEST_F(QStateTest, Constructors) {
  QState(5_in, 2_in);
  QState();

  QState pointCopy(point);
  QState copy(state);
}

TEST_F(QStateTest, ConstructorsSetMembers) {
  EXPECT_EQ(state.x.convert(inch), 5);
  EXPECT_EQ(state.y.convert(inch), 2);
  EXPECT_EQ(state.theta.convert(radian), 3);
}

TEST_F(QStateTest, CopyConstructorFunctions) {
  EXPECT_EQ(QState(state).x, state.x);
  EXPECT_EQ(QState(state).y, state.y);
  EXPECT_EQ(QState(state).theta, state.theta);
}

TEST_F(QStateTest, DefaultInitialization) {
  EXPECT_EQ(QState(), (QState {0_in, 0_in, 0_rad}));
}

TEST_F(QStateTest, MathOperators) {
  EXPECT_EQ((state + state).x, state.x * 2);
  EXPECT_EQ((state + state).y, state.y * 2);
  EXPECT_EQ((state + state).theta, state.theta * 2);

  EXPECT_EQ((state - state).x, 0_in);
  EXPECT_EQ((state - state).y, 0_in);
  EXPECT_EQ((state - state).theta, 0_rad);
}

TEST_F(QStateTest, EqualityOperators) {
  EXPECT_EQ(state, state);
  EXPECT_EQ(state + (QState {1_in, 2_in, 4_rad}), state + (QState {1_in, 2_in, 4_rad}));

  ASSERT_NE(state, state + state);
  ASSERT_NE(state, state + (QState {0_in, 0_in, 4_rad}));
}

TEST_F(QStateTest, AccessorOperator) {
  EXPECT_EQ(state[0], state.x);
  EXPECT_EQ(state[1], state.y);
  ASSERT_THROW(state[2], std::runtime_error);
  ASSERT_THROW(state[5], std::runtime_error);
  ASSERT_THROW(state[-1], std::runtime_error);
}

TEST_F(QStateTest, Conversions) {
  EXPECT_EQ(QPoint(QState(point)), point);
  EXPECT_EQ(point, QPoint(state));
}