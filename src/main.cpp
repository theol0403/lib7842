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
  GUI::Screen scr(lv_scr_act(), LV_COLOR_ORANGE);

  Controller controller(ControllerId::master);

  auto topLeft = std::make_shared<Motor>(11); // top left
  auto topRight = std::make_shared<Motor>(-5); // top right
  auto bottomRight = std::make_shared<Motor>(-6); // bottom right
  auto bottomLeft = std::make_shared<Motor>(2); // bottom left

  topLeft->setBrakeMode(AbstractMotor::brakeMode::brake);
  topRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomLeft->setBrakeMode(AbstractMotor::brakeMode::brake);

  /**
   * Model
   */
  auto model = std::make_shared<XDriveModel>(
    // motors
    topLeft, topRight, bottomRight, bottomLeft,
    // sensors
    std::make_shared<IntegratedEncoder>(11), std::make_shared<IntegratedEncoder>(-5),
    // limits
    200, 12000);

  /**
   * Trajectory
   */
  ChassisScales scales({3.25_in, 15_in}, 360);
  Limits limits(scales, 200_rpm, 1_s, std::sqrt(2), 1);
  XGenerator generator(model, 200_rpm, limits, scales, 10_ms);

  while (true) {
    model->xArcade(controller.getAnalog(ControllerAnalog::rightX),
                   controller.getAnalog(ControllerAnalog::rightY),
                   controller.getAnalog(ControllerAnalog::leftX));

    if (controller.getDigital(ControllerDigital::A)) {
      // generator.follow(Line({0_m, 0_m}, {0_m, 1.5_ft}));
      // generator.follow(Line({0_m, 0_m}, {0_m, 1.5_ft}), false);
      // generator.follow(CubicBezier({{0_ft, 0_ft}, {0.7_ft, 0_ft}, {0.7_ft, 1_ft}, {1.4_ft,
      // 1_ft}})); generator.follow(CubicBezier({{0_ft, 0_ft}, {0.7_ft, 0_ft}, {0.7_ft, 1_ft},
      // {1.4_ft, 1_ft}}),
      //                  false);

      // generator.follow(make_piecewise<QuinticHermite>(
      //   {{0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg}, {0_ft, 4_ft, 0_deg}}));

      generator.follow(Bezier<7>({{0_ft, 0_ft},
                                  {0_ft, 1.5_ft},
                                  {2_ft, 0.5_ft},
                                  {2_ft, 2_ft},
                                  {2_ft, 2_ft},
                                  {2_ft, 3.5_ft},
                                  {0_ft, 2.5_ft},
                                  {0_ft, 4_ft}}));
    }

    pros::delay(10);
  }
}
