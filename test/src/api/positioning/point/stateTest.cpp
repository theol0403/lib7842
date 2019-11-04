#include "test.hpp"

class StateTest : public ::testing::Test {
protected:
  State state {5_in, 2_in, 3_rad};
  Vector point {5_in, 2_in};
};

TEST_F(StateTest, Constructors) {
  State(5_in, 2_in);
  State();

  State pointCopy(point);
  State copy(state);
}

TEST_F(StateTest, ConstructorsSetMembers) {
  EXPECT_EQ(state.x.convert(inch), 5);
  EXPECT_EQ(state.y.convert(inch), 2);
  EXPECT_EQ(state.theta.convert(radian), 3);
}

TEST_F(StateTest, CopyConstructorFunctions) {
  EXPECT_EQ(State(state).x, state.x);
  EXPECT_EQ(State(state).y, state.y);
  EXPECT_EQ(State(state).theta, state.theta);
}

TEST_F(StateTest, DefaultInitialization) {
  EXPECT_EQ(State(), (State {0_in, 0_in, 0_rad}));
}

TEST_F(StateTest, MathOperators) {
  EXPECT_EQ((state + state).x, state.x * 2);
  EXPECT_EQ((state + state).y, state.y * 2);
  EXPECT_EQ((state + state).theta, state.theta * 2);

  EXPECT_EQ((state - state).x, 0_in);
  EXPECT_EQ((state - state).y, 0_in);
  EXPECT_EQ((state - state).theta, 0_rad);
}

TEST_F(StateTest, EqualityOperators) {
  EXPECT_EQ(state, state);
  EXPECT_EQ(state + (State {1_in, 2_in, 4_rad}), state + (State {1_in, 2_in, 4_rad}));

  ASSERT_NE(state, state + state);
  ASSERT_NE(state, state + (State {0_in, 0_in, 4_rad}));
}

TEST_F(StateTest, AccessorOperator) {
  EXPECT_EQ(state[0], state.x);
  EXPECT_EQ(state[1], state.y);
  ASSERT_THROW(state[2], std::runtime_error);
  ASSERT_THROW(state[5], std::runtime_error);
  ASSERT_THROW(state[-1], std::runtime_error);
}

TEST_F(StateTest, Conversions) {
  EXPECT_EQ(Vector(State(point)), point);
  EXPECT_EQ(point, Vector(state));

  [&](const Vector& ipoint) {
    EXPECT_EQ(ipoint, Vector(state));
  }(state);

  [&](const State& istate) {
    EXPECT_EQ(State(Vector(istate)), State(Vector(state)));
  }(state);

  [&](const State& istate) {
    EXPECT_EQ(State(Vector(istate)), State(point));
  }(State(point));
}