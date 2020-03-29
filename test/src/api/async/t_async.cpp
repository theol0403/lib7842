#include "../test/include/test.hpp"

TEST_CASE("Async") {

  SUBCASE("basic functionality") {
    bool ran = false;
    Async async([&]() { ran = true; });
    REQUIRE(!async.hasStarted());
    REQUIRE(!ran);
    async.waitUntilComplete();
    REQUIRE(async.isComplete());
    REQUIRE(ran);
  }

  SUBCASE("trigger functionality") {
    bool ran = false;
    Async async(Trigger().requirement([]() { return false; }), [&]() { ran = true; });
    REQUIRE(!async.hasStarted());
    REQUIRE(!ran);
    pros::delay(1);
    REQUIRE(!async.hasStarted());
    REQUIRE(!ran);
    async.forceStart();
    async.waitUntilComplete();
    REQUIRE(async.isComplete());
    REQUIRE(ran);
  }
}