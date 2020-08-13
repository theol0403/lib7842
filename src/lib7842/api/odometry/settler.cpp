#include "lib7842/api/odometry/settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"
#include <iostream>

namespace lib7842 {

Settler&& Settler::distanceTo(const Vector& point, const QLength& trigger) {
  assembly.emplace([=, this]() { requirement(controller->distanceTo(point, trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const Vector& point, const QAngle& trigger) {
  assembly.emplace([=, this]() { requirement(controller->angleTo(point, trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const QAngle& angle, const QAngle& trigger) {
  assembly.emplace([=, this]() { requirement(controller->angleTo(angle, trigger)); });
  return std::move(*this);
}

Settler&& Settler::distanceErr(const QLength& trigger) {
  assembly.emplace([=, this]() { requirement(controller->distanceErr(trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleErr(const QAngle& trigger) {
  assembly.emplace([=, this]() { requirement(controller->angleErr(trigger)); });
  return std::move(*this);
}

Settler&& Settler::distanceSettled() {
  assembly.emplace([=, this]() { requirement(controller->distanceSettled()); });
  return std::move(*this);
}

Settler&& Settler::turnSettled() {
  assembly.emplace([=, this]() { requirement(controller->turnSettled()); });
  return std::move(*this);
}

Settler&& Settler::angleSettled() {
  assembly.emplace([=, this]() { requirement(controller->angleSettled()); });
  return std::move(*this);
}

Settler&& Settler::distanceSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace([=, this]() { requirement(controller->distanceSettledUtil(timeUtil)); });
  return std::move(*this);
}

Settler&& Settler::angleSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace([=, this]() { requirement(controller->angleSettledUtil(timeUtil)); });
  return std::move(*this);
}

Settler&& Settler::abort(const TimeUtil& itimeUtil) {
  driveAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
  return std::move(*this);
}

Settler&& Settler::noAbort() {
  driveAbort = nullptr;
  return std::move(*this);
}

bool Settler::run(const OdomController* icontroller) {
  controller = icontroller;

  while (!assembly.empty()) {
    assembly.top()();
    assembly.pop();
  }

  auto error = controller->getDistanceError();
  auto change = error - lastError;
  lastError = error;

  if (driveAbort && driveAbort->isSettled(change.convert(millimeter))) {
    GLOBAL_WARN_S("Settler::run: Aborting drive command");
    return true;
  }
  return Trigger::run();
}

bool Settler::operator()(const OdomController* icontroller) { return run(icontroller); }

void Settler::setDefaultAbort(const TimeUtil& itimeUtil) {
  defaultAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

std::shared_ptr<SettledUtil> Settler::defaultAbort {nullptr};

} // namespace lib7842

#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/test/mocks.hpp"
namespace test {
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

  SUBCASE("nothing should return true") { REQUIRE(settler(chassis.get())); }

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
} // namespace test
