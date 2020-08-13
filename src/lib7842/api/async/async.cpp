#include "lib7842/api/async/async.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Async::Async(std::function<void()>&& iaction) : action(std::move(iaction)) { startTask("Async"); }

Async::Async(Trigger&& itrigger, std::function<void()>&& iaction) :
  trigger(std::move(itrigger)), action(std::move(iaction)) {
  startTask("Async");
}

void Async::forceStart() { _forceStart = true; }

void Async::forceStop() {
  stopTask();
  _complete = true;
}

bool Async::hasStarted() const { return _started; }

bool Async::isComplete() const { return _complete; }

void Async::waitUntilComplete() const {
  while (!_complete) {
    pros::delay(10);
  }
}

void Async::loop() {
  while (!(trigger() || _forceStart)) {
    pros::delay(10);
  }
  _started = true;
  action();
  _complete = true;
}

} // namespace lib7842

#include "lib7842/test/test.hpp"
namespace test {
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
} // namespace test
