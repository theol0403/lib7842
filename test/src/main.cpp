#include "lib7842/api.hpp"
#include "lib7842/api/other/global.hpp"
#include "lib7842/test/mocks.hpp"
#include <iostream>
#include <sys/unistd.h>

using namespace test;
int lvglMain();

int main(int argc, char** argv) {
  global::setTimeUtil(std::make_shared<TimeUtil>(createTimeUtil()));

  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running lvgl:" << std::endl;
    return lvglMain();
  }

  ChassisScales scales({3.25_in, 13_in}, 360);
  Limits limits(scales, 200_rpm, 0.6_s);
  XGenerator generator(nullptr, 200_rpm, scales, limits, 10_ms);

  // auto [profile, t] =
  //   generator.follow(Bezier<3>({{0_ft, 0_ft}, {0_ft, 3_ft}, {2_ft, 1_ft}, {2_ft, 4_ft}}));

  // auto [profile, t] = generator.follow(Mesh({0_m, 0_m, 0_deg}, {1_ft, 3_ft, 60_deg}));

  // auto [profile, t] = generator.follow(Line({0_m, 0_m}, {1_ft, 0_ft}));

  auto [profile, t] = generator.follow(QuinticHermite({0_ft, 0_ft, 0_deg}, {2_ft, 4_ft, 0_deg}));

  if (argc > 1 && std::string(argv[1]) == "print") {
    for (auto&& step : t) {
      std::cout << step.p.x.convert(foot) << "," << step.p.y.convert(foot) << ","
                << step.p.theta.convert(degree) << "," << step.c.convert(1 / meter) << ","
                << step.p_vel << "," << step.k.v << "," << step.w.convert(degree / second) << ","
                << step.left << "," << step.right << "," << step.leftBack << "," << step.rightBack
                << "," << step.robot << std::endl;
    }
  } else {
    std::cout << std::endl;
    std::cout << "Velocity: " << limits.v << " m/s" << std::endl;
    std::cout << "Acceleration: " << limits.a << " m/s2" << std::endl;
    std::cout << "Angular Velocity: " << limits.w.convert(degree / second) << " deg/s" << std::endl;
    std::cout << std::endl;
    std::cout << "Length: " << profile.end().d.convert(foot) << " ft" << std::endl;
    std::cout << "Time: " << profile.end().t << " s" << std::endl;
  }

  // return runUnitTests(argc, argv);

  return 0;
}

extern "C" {
namespace pros::c {
void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }
} // namespace pros::c
} // extern "C"
