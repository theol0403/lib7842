#include <gtest/gtest.h>
#include "lib7842/purePursuit/point.hpp"

TEST(QPointTest, ConstructorsSetMembers) {
  lib7842::QPoint point ({5_in, 2_in, 1_rad});

  EXPECT_EQ(point.x.convert(inch), 5);
  EXPECT_EQ(point.y.convert(inch), 2);
  EXPECT_EQ(point.theta.convert(radian), 1);
}

TEST(QPointTest, CopyConstructorFunctions) {
  lib7842::QPoint point ({5_in, 2_in, 1_rad});

  EXPECT_EQ(lib7842::QPoint(point).x, point.x);
  EXPECT_EQ(lib7842::QPoint(point).y, point.y);
  EXPECT_EQ(lib7842::QPoint(point).theta, point.theta);
}

TEST(QPointTest, VariousConstructors) {

  EXPECT_EQ(lib7842::QPoint(5_in, 2_in).x, 5_in);
  EXPECT_EQ(lib7842::QPoint(5_in, 2_in).y, 2_in);
  EXPECT_EQ(lib7842::QPoint(5_in, 2_in).theta, 0_rad);

  EXPECT_EQ(lib7842::QPoint().theta, 0_rad);
}

TEST(QPointTest, AddOperator) {
  lib7842::QPoint point ({5_in, 2_in, 1_rad});

  EXPECT_EQ((point + point).x, point.x*2);
  EXPECT_EQ((point + point).y, point.y*2);
  EXPECT_EQ((point + point + point).theta, point.theta*3);
}


TEST(DPointTest, ConstructorsSetMembers) {
  lib7842::dPoint point ({5, 2, 1});

  EXPECT_EQ(point.x, 5);
  EXPECT_EQ(point.y, 2);
  EXPECT_EQ(point.theta, 1);
}

TEST(DPointTest, CopyConstructorFunctions) {
  lib7842::dPoint point ({5, 2, 1});

  EXPECT_EQ(lib7842::dPoint(point).x, point.x);
  EXPECT_EQ(lib7842::dPoint(point).y, point.y);
  EXPECT_EQ(lib7842::dPoint(point).theta, point.theta);
}

TEST(DPointTest, VariousConstructors) {

  EXPECT_EQ(lib7842::dPoint(5, 2).x, 5);
  EXPECT_EQ(lib7842::dPoint(5, 2).y, 2);
  EXPECT_EQ(lib7842::dPoint(5, 2).theta, 0);

  EXPECT_EQ(lib7842::dPoint().theta, 0);
}

TEST(DPointTest, AddOperator) {
  lib7842::dPoint point ({5, 2, 1});

  EXPECT_EQ((point + point).x, point.x*2);
  EXPECT_EQ((point + point).y, point.y*2);
  EXPECT_EQ((point + point + point).theta, point.theta*3);
}