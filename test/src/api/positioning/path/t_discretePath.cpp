#include "../test/include/test.hpp"

double checkLogic(int path_l, int angle_l, int index) {
  if (index == 0) { return 0; }
  if (index == path_l - 1) { return angle_l - 1; }

  double ratio = static_cast<double>(angle_l - 1) / static_cast<double>(path_l - 1);

  return index * ratio;
}

TEST_CASE("checkLogic") {
  REQUIRE(checkLogic(1, 1, 0) == 0);

  REQUIRE(checkLogic(2, 1, 0) == 0);
  REQUIRE(checkLogic(2, 1, 1) == 0);

  REQUIRE(checkLogic(3, 1, 0) == 0);
  REQUIRE(checkLogic(3, 1, 1) == 0);
  REQUIRE(checkLogic(3, 1, 2) == 0);

  REQUIRE(checkLogic(4, 1, 0) == 0);
  REQUIRE(checkLogic(4, 1, 1) == 0);
  REQUIRE(checkLogic(4, 1, 2) == 0);
  REQUIRE(checkLogic(4, 1, 3) == 0);

  REQUIRE(checkLogic(2, 2, 0) == 0);
  REQUIRE(checkLogic(2, 2, 1) == 1);

  REQUIRE(checkLogic(3, 2, 0) == 0);
  REQUIRE(checkLogic(3, 2, 1) == 0.5);
  REQUIRE(checkLogic(3, 2, 2) == 1);

  REQUIRE(checkLogic(4, 2, 0) == 0);
  REQUIRE(checkLogic(4, 2, 1) == 1.0 / 3.0);
  REQUIRE(checkLogic(4, 2, 2) == 2.0 / 3.0);
  REQUIRE(checkLogic(4, 2, 3) == 1);

  REQUIRE(checkLogic(2, 3, 0) == 0);
  REQUIRE(checkLogic(2, 3, 1) == 2);

  REQUIRE(checkLogic(3, 3, 0) == 0);
  REQUIRE(checkLogic(3, 3, 1) == 1);
  REQUIRE(checkLogic(3, 3, 2) == 2);

  REQUIRE(checkLogic(4, 3, 0) == 0);
  REQUIRE(checkLogic(4, 3, 1) == 2.0 / 3.0);
  REQUIRE(checkLogic(4, 3, 2) == 4.0 / 3.0);
  REQUIRE(checkLogic(4, 3, 3) == 2);

  REQUIRE(checkLogic(2, 4, 0) == 0);
  REQUIRE(checkLogic(2, 4, 1) == 3);

  REQUIRE(checkLogic(3, 4, 0) == 0);
  REQUIRE(checkLogic(3, 4, 1) == 1.5);
  REQUIRE(checkLogic(3, 4, 2) == 3);

  REQUIRE(checkLogic(4, 4, 0) == 0);
  REQUIRE(checkLogic(4, 4, 1) == 1);
  REQUIRE(checkLogic(4, 4, 2) == 2);
  REQUIRE(checkLogic(4, 4, 3) == 3);
}
