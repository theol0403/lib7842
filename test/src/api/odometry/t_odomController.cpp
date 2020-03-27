#include "../test/include/test.hpp"

class MockOdomController : public OdomController {
public:
  using OdomController::OdomController;
};

TEST_CASE("OdomController") {

  auto model = std::make_shared<MockThreeEncoderXDriveModel>();
  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));
  auto chassis = std::make_shared<MockOdomController>(
    model, odom, std::make_unique<IterativePosPIDController>(0.015, 0, 0, 0, createTimeUtil()),
    std::make_unique<IterativePosPIDController>(0.03, 0, 0, 0, createTimeUtil()),
    std::make_unique<IterativePosPIDController>(0.02, 0, 0, 0, createTimeUtil()), 0_in);

  SUBCASE("moving with default settler should not segfault") {
    chassis->driveToPoint({0_in, 0_in});
  }

  SUBCASE("turning with default settler should not segfault") {
    chassis->turnAngle(0_deg);
  }
}