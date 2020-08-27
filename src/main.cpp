#include "main.h"

#include "lib7842/api.hpp"
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
  Controller controller(ControllerId::master);

  auto topLeft = std::make_shared<Motor>(1); // top left
  auto topRight = std::make_shared<Motor>(-8); // top right
  auto bottomRight = std::make_shared<Motor>(-3); // bottom right
  auto bottomLeft = std::make_shared<Motor>(6); // bottom left

  topLeft->setBrakeMode(AbstractMotor::brakeMode::brake);
  topRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomRight->setBrakeMode(AbstractMotor::brakeMode::brake);
  bottomLeft->setBrakeMode(AbstractMotor::brakeMode::brake);

  /**
   * Model
   */
  auto model = std::make_shared<ThreeEncoderXDriveModel>(
    // motors
    topLeft, topRight, bottomRight, bottomLeft,
    // sensors
    std::make_shared<ADIEncoder>(1, 2, true), //
    std::make_shared<ADIEncoder>(5, 6, true), //
    std::make_shared<ADIEncoder>(3, 4, true), //
    // limits
    200, 12000);

  ChassisScales scales({2.75_in, 11.3_in, 0_in, 2.75_in}, 360);

  /**
   * Odom
   */
  auto odom = std::make_shared<CustomOdometry>(model, scales);
  odom->startTask("Odometry");

  /**
   * Controller
   */
  auto odomController = std::make_shared<OdomXController>(
    model, odom,
    // Distance PID - To mm
    std::make_unique<IterativePosPIDController>(
      0.0165, 0.00026, 0.00033, 0, TimeUtilFactory::withSettledUtilParams(10, 5, 150_ms)),
    // Turn PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.045, 0.002, 0.0006, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)),
    // Angle PID - To Degree
    std::make_unique<IterativePosPIDController>(
      0.043, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(2, 1, 150_ms)),
    0_ft);

  /**
   * Screen
   */
  GUI::Screen scr(lv_scr_act(), LV_COLOR_ORANGE);
  scr.makePage<GUI::Odom>("Odom").attachOdom(odom).attachResetter([&] { odom->reset(); });

  /**
   * Follower
   */
  PathFollower follower(model, odom, ChassisScales({2.75_in, 11.5_in}, imev5GreenTPR), 200_rpm,
                        1.3_ft);

  PursuitLimits limits {2.75_in, 200_rpm, 0.1, 2_s, 1};

  while (true) {
    model->xArcade(controller.getAnalog(ControllerAnalog::rightX),
                   controller.getAnalog(ControllerAnalog::rightY),
                   controller.getAnalog(ControllerAnalog::leftX));

    if (controller.getDigital(ControllerDigital::A)) {

      // auto path =
      //   QuinticPath({{0_ft, 0_ft, 0_deg}, {-1_ft, 2_ft, -90_deg}, {-2_ft, 0_ft, 180_deg}}, 1.1)
      //     .generate(100);

      // follower.followPath(PathGenerator::generate(path, limits));

      // auto path2 = QuinticPath({{-2_ft, 0_ft, 0_deg}, {0_ft, 2_ft, 0_deg}}, 2).generate(100);

      // follower.followPath(PathGenerator::generate(path2, limits), true);

      auto p = QuinticHermite({0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg})
                 .step(StepBy::ConstCount<100>())
                 .generate();

      // auto p =
      //   Stepper(QuinticHermite({0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg}), StepBy::Count(100));

      follower.followPath(PathGenerator::generate(p, limits), limits, false);
    }

    pros::delay(10);
  }
}
