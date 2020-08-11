#include "lib7842/api/async/trigger.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Trigger::Function Trigger::Function::operator!() && { return std::not_fn(std::move(*this)); }

Trigger&& Trigger::requirement(Trigger::Function&& function) {
  requirements.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::require(Trigger::Function&& function) {
  return requirement(std::move(function));
}

Trigger&& Trigger::exception(Trigger::Function&& function) {
  exceptions.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::unless(Trigger::Function&& function) { return exception(std::move(function)); }

bool Trigger::run() {
  if (std::any_of(exceptions.begin(), exceptions.end(),
                  [](const auto& function) { return function(); })) {
    return true;
  }
  if (std::all_of(requirements.begin(), requirements.end(),
                  [](const auto& function) { return function(); })) {
    return true;
  }
  return false;
}

bool Trigger::operator()() { return run(); }

Trigger::Function Trigger::time(const QTime& time) {
  return [=, timer = std::shared_ptr<AbstractTimer>(global::getTimeUtil()->getTimer())]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() >= time;
  };
}

} // namespace lib7842

#include "lib7842/test.hpp"
namespace test {
// class MockOdomController : public OdomController {
// public:
//   using OdomController::OdomController;
//   using OdomController::_distanceErr;
// };

TEST_CASE("Trigger") {
  Trigger trigger;

  SUBCASE("logic tests") {

    SUBCASE("nothing should return true") { REQUIRE(trigger()); }

    SUBCASE("requirements") {
      SUBCASE("adding true") {
        trigger.requirement([] { return true; });
        REQUIRE(trigger());

        SUBCASE("adding exception") {
          trigger.exception([] { return true; });
          REQUIRE(trigger());
        }

        SUBCASE("adding true") {
          trigger.requirement([] { return true; });
          REQUIRE(trigger());

          SUBCASE("adding false") {
            trigger.requirement([] { return false; });
            REQUIRE(!trigger());

            SUBCASE("adding exception") {
              trigger.exception([] { return true; });
              REQUIRE(trigger());
            }
          }
        }
      }

      SUBCASE("adding false") {
        trigger.requirement([] { return false; });
        REQUIRE(!trigger());

        SUBCASE("adding true") {
          trigger.requirement([] { return true; });
          REQUIRE(!trigger());

          SUBCASE("adding exception") {
            trigger.exception([] { return true; });
            REQUIRE(trigger());
          }
        }
      }

      SUBCASE("adding exception") {
        trigger.requirement([] { return false; });
        trigger.exception([] { return false; });
        REQUIRE(!trigger());

        SUBCASE("adding exception") {
          trigger.exception([] { return true; });
          REQUIRE(trigger());
        }
      }
    }
  }

  // SUBCASE("chassis tests") {
  //   auto model = std::make_shared<MockThreeEncoderXDriveModel>();
  //   auto odom =
  //     std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));
  //   auto chassis =
  //     std::make_shared<MockOdomController>(model, odom, nullptr, nullptr, nullptr, 0_in);

  //   SUBCASE("distanceErr") {
  //     trigger.require(chassis->distanceErr(5_mm));
  //     chassis->_distanceErr = 10_mm;
  //     REQUIRE(!trigger());
  //     chassis->_distanceErr = 1_mm;
  //     REQUIRE(trigger());
  //   }

  //   SUBCASE("settledUtil") {
  //     trigger.require(
  //       chassis->distanceSettledUtil(createTimeUtil(Supplier<std::unique_ptr<SettledUtil>>(
  //         []() { return createSettledUtilPtr(5, 0, 20_ms); }))));
  //     chassis->_distanceErr = 10_mm;
  //     REQUIRE(!trigger());
  //     pros::delay(30);
  //     REQUIRE(!trigger());
  //     chassis->_distanceErr = 1_mm;
  //     REQUIRE(!trigger());
  //     REQUIRE(!trigger()); // clear the derivative
  //     pros::delay(30);
  //     REQUIRE(trigger());
  //   }
  // }

  SUBCASE("max time") {
    trigger.require(Trigger::time(20_ms));
    pros::delay(30);
    REQUIRE(!trigger());
    pros::delay(30);
    REQUIRE(trigger());
  }
}
} // namespace test
