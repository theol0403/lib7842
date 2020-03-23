#include "test.hpp"

class MockOdomController : public OdomController {
public:
  using OdomController::OdomController;
  using OdomController::distanceErr;
};

TEST_CASE("Settler test") {
  auto model = std::make_shared<MockThreeEncoderXDriveModel>();
  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));
  auto chassis = std::make_shared<MockOdomController>(model, odom, nullptr, nullptr, nullptr, 0_in);

  Settler trigger = chassis->settler();

  SUBCASE("nothing should return false") {
    REQUIRE(!trigger());
  }

  trigger.requirement([] { return false; });

  trigger.abort(createTimeUtil(
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(5, 0, 20_ms); })));

  SUBCASE("checking abort") {
    pros::delay(30);
    REQUIRE(!trigger());
    pros::delay(30);
    REQUIRE(trigger());

    trigger.noAbort();
    REQUIRE(!trigger());
  }

  SUBCASE("checking not abort") {
    pros::delay(30);
    REQUIRE(!trigger());
    chassis->distanceErr = 10_mm;
    pros::delay(30);
    REQUIRE(!trigger());
    chassis->distanceErr = 0_mm;
    pros::delay(30);
    REQUIRE(!trigger());
  }
}
