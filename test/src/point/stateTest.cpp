#include "test.hpp"
#include "lib7842/point/state.hpp"

class QStateTest : public ::testing::Test {
protected:
  QState state {5_in, 2_in, 3_rad};
  Vector point {5_in, 2_in};
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
  EXPECT_EQ(Vector(QState(point)), point);
  EXPECT_EQ(point, Vector(state));

  [&](const Vector& ipoint) {
    EXPECT_EQ(ipoint, Vector(state));
  }(state);

  [&](const QState& istate) {
    EXPECT_EQ(QState(Vector(istate)), QState(Vector(state)));
  }(state);

  [&](const QState& istate) {
    EXPECT_EQ(QState(Vector(istate)), QState(point));
  }(QState(point));
}