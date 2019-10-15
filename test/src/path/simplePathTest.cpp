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
