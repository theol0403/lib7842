#include "../test/include/test.hpp"

class MockOdomController : public OdomController {
public:
  using OdomController::OdomController;
  using OdomController::_distanceErr;
};

TEST_CASE("Settler") {
  auto model = std::make_shared<MockThreeEncoderXDriveModel>();
  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));
  auto chassis = std::make_shared<MockOdomController>(model, odom, nullptr, nullptr, nullptr, 0_in);

  Settler settler;

  SUBCASE("nothing should return false") {
    REQUIRE(!settler(chassis.get()));
  }

  settler.requirement([] { return false; });

  settler.abort(createTimeUtil(
    Supplier<std::unique_ptr<SettledUtil>>([]() { return createSettledUtilPtr(5, 0, 20_ms); })));

  SUBCASE("checking abort") {
    pros::delay(30);
    REQUIRE(!settler(chassis.get()));
    pros::delay(30);
    REQUIRE(settler(chassis.get()));

    settler.noAbort();
    REQUIRE(!settler(chassis.get()));
  }

  SUBCASE("checking not abort") {
    pros::delay(30);
    REQUIRE(!settler(chassis.get()));
    chassis->_distanceErr = 10_mm;
    pros::delay(30);
    REQUIRE(!settler(chassis.get()));
    chassis->_distanceErr = 0_mm;
    pros::delay(30);
    REQUIRE(!settler(chassis.get()));
  }
}
