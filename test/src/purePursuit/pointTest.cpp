#include "test.hpp"
#include "lib7842/purePursuit/point.hpp"

QPoint point(5_in, 2_in);
QPoint emptyPoint;

TEST(QPointTest, Constructor) {
  EXPECT_EQ(point.x.convert(inch), 5);
  EXPECT_EQ(point.y.convert(inch), 2);
}

TEST(QPointTest, CopyConstructor) {
  EXPECT_EQ(QPoint(point), point);
}

TEST(QPointTest, DefaultInitialization) {
  EXPECT_EQ(emptyPoint, (QPoint {0_in, 0_in}));
}

TEST(QPointTest, Operators) {
  EXPECT_EQ((point + point).x, point.x * 2);
  EXPECT_EQ((point + point).y, point.y * 2);

  EXPECT_EQ((point - point), emptyPoint);
}

QState state(5_in, 2_in, 3_rad);

TEST(QStateTest, Constructors) {
  //inherited
  QState(5_in, 2_in);
  QState();

  QState pointCopy(point);
  QState copy(state);
}

TEST(QStateTest, ConstructorsSetMembers) {
  EXPECT_EQ(state.x.convert(inch), 5);
  EXPECT_EQ(state.y.convert(inch), 2);
  EXPECT_EQ(state.theta.convert(radian), 3);
}

TEST(QStateTest, CopyConstructorFunctions) {
  EXPECT_EQ(QState(state).x, state.x);
  EXPECT_EQ(QState(state).y, state.y);
  EXPECT_EQ(QState(state).theta, 3_rad);
}

TEST(QStateTest, DefaultInitialization) {
  EXPECT_EQ(QState(), (QState {0_in, 0_in, 0_rad}));
}

TEST(QStateTest, Operators) {
  EXPECT_EQ((state + state).x, state.x * 2);
  EXPECT_EQ((state + state).y, state.y * 2);

  EXPECT_EQ((state - state).x, 0_in);
  EXPECT_EQ((state - state).y, 0_in);
}