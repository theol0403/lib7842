#include "main.h"

#include "lib7842/api.hpp"
#include "lib7842/api/positioning/spline/piecewise.hpp"
#include "lib7842/api/positioning/spline/stepper.hpp"
#include "lib7842/test/test.hpp"

using namespace lib7842;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::delay(200);
  test::runUnitTests(0, nullptr);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
  /**
   * Screen
   */
  // GUI::Screen scr(lv_scr_act(), LV_COLOR_ORANGE);

  Controller controller(ControllerId::master);

  auto topLeft = std::make_shared<Motor>(11); // top left
  auto topRight = std::make_shared<Motor>(-18); // top right
  auto bottomRight = std::make_shared<Motor>(-19); // bottom right
  auto bottomLeft = std::make_shared<Motor>(2); // bottom left

  topLeft->setBrakeMode(AbstractMotor::brakeMode::brake);
  topRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomLeft->setBrakeMode(AbstractMotor::brakeMode::brake);

  auto model = std::make_shared<XDriveModel>(
    // motors
    topLeft, topRight, bottomRight, bottomLeft,
    // sensors
    std::make_shared<IntegratedEncoder>(11), std::make_shared<IntegratedEncoder>(-5),
    // limits
    200, 12000);

  ChassisScales scales({3.25_in, 16_in}, 360);
  Limits<> limits(scales, 200_rpm, 0.7_s, 1, 1);

  XGenerator generator(model, 200_rpm, scales, limits, 10_ms);
  // SkidSteerGenerator sgenerator(model, 200_rpm, scales, limits, 10_ms);

  while (true) {
    model->xArcade(controller.getAnalog(ControllerAnalog::rightX),
                   controller.getAnalog(ControllerAnalog::rightY),
                   controller.getAnalog(ControllerAnalog::leftX));

    if (controller.getDigital(ControllerDigital::A)) {
      // auto [profile, t] = generator.follow(Mesh({0_m, 0_m, 0_deg}, {1_ft, 3_ft, 60_deg}));
      // auto [profile, t] =
      //   generator.follow(Bezier<3>({{0_ft, 0_ft}, {0_ft, 3_ft}, {2_ft, 1_ft}, {2_ft, 4_ft}}));
      // auto [profile, t] =
      //   generator.follow(QuinticHermite({0_ft, 0_ft, 0_deg}, {2_ft, 4_ft, 0_deg}));
      auto [profile, t] = generator.follow(
        Bezier<3>({{0_ft, 0_ft}, {0_ft, -1.5_ft}, {-2_ft, -1.5_ft}, {-2.5_ft, 0.5_ft}}),
        {.rotator = makeAngler(40_deg, Limits<QAngle>(0.5_s, 50_deg / second))});
    }

    pros::delay(10);
  }
}
