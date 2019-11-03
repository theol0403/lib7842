#include "main.h"
#include "lib7842/gui/screen.hpp"
#include "lib7842/gui/odomDebug.hpp"
#include "lib7842/odometry/customOdometry.hpp"
#include "lib7842/odometry/odomController.hpp"
#include "lib7842/odometry/odomXController.hpp"

using namespace lib7842;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::delay(200);
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
  Controller controller(ControllerId::master);

  auto model = std::make_shared<ThreeEncXDriveModel>(
    // motors
    std::make_shared<Motor>(1), //
    std::make_shared<Motor>(-2), //
    std::make_shared<Motor>(-3), //
    std::make_shared<Motor>(4), //
    // sensors
    std::make_shared<ADIEncoder>(3, 4, true), //
    std::make_shared<ADIEncoder>(5, 6), //
    std::make_shared<ADIEncoder>(1, 2, true), //
    // limits
    200, 12000);

  auto odom =
    std::make_shared<CustomOdometry>(model, ChassisScales({2.75_in, 12.9473263_in, 0.00_in}, 360));
  odom->startTask("Odometry");

  Screen scr(lv_scr_act(), LV_COLOR_ORANGE);
  scr.makePage<OdomDebug>().attachOdom(odom);
  scr.startTask("Screen");

  auto odomController = std::make_shared<OdomXController>(
    model, odom,
    //Distance PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.015, 0.0002, 0.0002, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    //Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.03, 0.00, 0.0003, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    //Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.02, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 10, 100_ms)),
    //Strafe PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.015, 0.0002, 0.0002, 0, TimeUtilFactory::withSettledUtilParams(10, 10, 100_ms)),
    2_in);

  while (true) {
    model->xArcade(
      controller.getAnalog(ControllerAnalog::rightX), controller.getAnalog(ControllerAnalog::rightY),
      controller.getAnalog(ControllerAnalog::leftX));

    if (controller.getDigital(ControllerDigital::A)) {
      // odomController->driveToPoint2({0_ft, 0_ft}, 2);
      odomController->strafeToPoint({0_ft, 0_ft}, OdomController::makeAngleCalculator(360));
    }

    pros::delay(10);
  }
}
