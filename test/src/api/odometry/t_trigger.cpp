#include "test.hpp"

TEST_CASE("Trigger test") {
  auto model = std::make_shared<MockThreeEncoderXDriveModel>();
  auto odom = std::make_shared<CustomOdometry>(
    model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}), createTimeUtil());
  auto chassis = std::make_shared<OdomController>(model, odom, nullptr, nullptr, nullptr, 0_in,
                                                  createTimeUtil());

  auto trigger = chassis->trigger();

  SUBCASE("nothing should return false") {
    REQUIRE(!trigger());
  }
}
