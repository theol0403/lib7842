#include "../test/include/test.hpp"

class MockOdomController : public OdomController {
public:
  using OdomController::OdomController;
  using OdomController::_distanceErr;
};

TEST_CASE("Trigger") {
  Trigger trigger;

  SUBCASE("logic tests") {

    SUBCASE("nothing should return true") {
      REQUIRE(trigger());
    }

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

  SUBCASE("chassis tests") {
    auto model = std::make_shared<MockThreeEncoderXDriveModel>();
    auto odom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));
    auto chassis =
      std::make_shared<MockOdomController>(model, odom, nullptr, nullptr, nullptr, 0_in);

    SUBCASE("distanceErr") {
      trigger.require(chassis->distanceErr(5_mm));
      chassis->_distanceErr = 10_mm;
      REQUIRE(!trigger());
      chassis->_distanceErr = 1_mm;
      REQUIRE(trigger());
    }

    SUBCASE("settledUtil") {
      trigger.require(
        chassis->distanceSettledUtil(createTimeUtil(Supplier<std::unique_ptr<SettledUtil>>(
          []() { return createSettledUtilPtr(5, 0, 20_ms); }))));
      chassis->_distanceErr = 10_mm;
      REQUIRE(!trigger());
      pros::delay(30);
      REQUIRE(!trigger());
      chassis->_distanceErr = 1_mm;
      REQUIRE(!trigger());
      REQUIRE(!trigger()); // clear the derivative
      pros::delay(30);
      REQUIRE(trigger());
    }
  }

  SUBCASE("max time") {
    trigger.require(Trigger::time(20_ms));
    pros::delay(30);
    REQUIRE(!trigger());
    pros::delay(30);
    REQUIRE(trigger());
  }
}
