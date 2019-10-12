#include "test.hpp"
#include "lib7842/path/simplePath.hpp"

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

TEST_F(SimplePathTest, ExtractReference) {
  SimplePath path({point1, point1});

  ReferencePath ipath = path.extractRef();
  ASSERT_EQ(ipath.get().size(), 2);
  for (auto&& point : ipath.get()) {
    ASSERT_EQ(point.get(), point1);
  }

  ReferencePath ipath2 = path.extractRef();
  for (size_t i = 0; i < ipath2.get().size(); i++) {
    ASSERT_EQ(&ipath.get()[i].get(), &ipath2.get()[i].get());
    ASSERT_EQ(&ipath.get()[i].get(), &path.get()[i]);
  }
}

TEST_F(SimplePathTest, ExtractPath) {
  SimplePath path({point1, point1, point1});

  SimplePath ipath = path.extract();
  ASSERT_EQ(ipath.get().size(), 3);
  for (auto&& point : ipath.get()) {
    ASSERT_EQ(point, point1);
  }
}
