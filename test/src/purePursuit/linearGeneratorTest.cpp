#include "test.hpp"
#include "lib7842/purePursuit/linearGenerator.hpp"

class LinearGeneratorTest : public ::testing::Test {
 protected:
  LinearGenerator::smoothParams_t smoothGains {0.25, 0.0001_in};
};

TEST_F(LinearGeneratorTest, InsertPoints) {
  SimplePath path =
    LinearGenerator::insert(SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 2_in}}), 0.5_in);
  ASSERT_EQ(path().size(), 20);
}

TEST_F(LinearGeneratorTest, SmoothPointsDontCopy) {
  SimplePath path =
    LinearGenerator::smoothen(SimplePath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}}), smoothGains);
  ASSERT_NE(std::round(path()[1].x.convert(inch) * 10) / 10, 2.5);
}

TEST_F(LinearGeneratorTest, SmoothPointsDontPassWayPoints) {
  SimplePath ipath({{0_in, 0_in}, {5_in, 4_in}, {5_in, 0_in}, {3_in, 5_in}, {0_in, 10_in}});
  SimplePath path = LinearGenerator::smoothen(ipath, smoothGains);
  for (size_t i = 1; i < path().size() - 1; ++i) {
    ASSERT_NE(path()[i], ipath()[i]);
  }
}

TEST_F(LinearGeneratorTest, SmoothPointsFunction) {
  LinearGenerator generator(SimplePath({{1_in, 1_in}, {5_in, 4_in}, {9_in, 1_in}}), smoothGains);

  SimplePath path = generator.generate(0.5_in);

  ASSERT_NEAR(path()[10].x.convert(inch), 5, 0.1);
  ASSERT_NEAR(path()[10].y.convert(inch), 3.5, 0.3);
}
