#include "../test/include/test.hpp"

TEST_CASE("Async") {

  SUBCASE("basic functionality") {
    bool ran = false;
    Async async([&]() { ran = true; });
    REQUIRE(!async.hasStarted());
    async.waitUntilComplete();
    REQUIRE(async.isComplete());
  }
}