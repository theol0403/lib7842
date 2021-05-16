#include "lib7842/api.hpp"
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/positioning/spline/bezier.hpp"
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
  Limits<> limits(scales, 200_rpm, 0.6_s);
  XGenerator generator(nullptr, 200_rpm, scales, limits, 10_ms);

#define move auto [profile, t] = generator.follow

  // move(QuarticBezier({{0_ft, 0_ft}, {0_ft, 1_ft}, {-2_ft, 2_ft}, {2_ft, 4_ft}, {2_ft, 6_ft}}),
  //      {.curve = true, .start = 90_deg});
  // move(QuinticHermite(-180_deg, {-0.1_ft, -2.0_ft, -45_deg}, 1.1),
  //      {.rotator = makeAngler(-42_deg, Limits<QAngle>(0.5_s, 60_deg / second))});
  // move(QuinticHermite({0_ft, 0_ft, 135_deg}, {-3.6_ft, 3.7_ft, 180_deg}),
  //      {.curve = true, .start = -45_deg});
  // move(QuinticHermite(110_deg, {3.2_ft, 2.0_ft, 0_deg}, 1.7, 3), {
  //                                                                  .curve = true,
  //                                                                });

  // move(QuinticHermite({0_ft, 0_ft, -180_deg}, {-0.1_ft, -2.0_ft, -45_deg}, 1.1),
  //      {.rotator = makeAngler(-42_deg, Limits<QAngle>(0.5_s, 60_deg / second))});

  move(QuinticHermite(225_deg, {-2.5_ft, -0.3_ft, 90_deg}, 2, 1.5),
       {.start = 45_deg, .rotator = makeAngler(39_deg, Limits<QAngle>(0.5_s, 60_deg / second))});

  if (argc > 1) {
    if (std::string(argv[1]) == "print") {
      for (auto&& step : t) {
        std::cout << step.p.x.convert(foot) << "," << step.p.y.convert(foot) << ","
                  << step.p.theta.convert(degree) << "," << step.c.convert(1 / meter) << ","
                  << step.p_vel << "," << step.k.v << "," << step.w.convert(degree / second) << ","
                  << step.left << "," << step.right << "," << step.leftBack << "," << step.rightBack
                  << std::endl;
      }
    } else if (std::string(argv[1]) == "bench") {
      for (size_t i = 0; i < 1000; i++) {
        move(QuarticBezier({{0_ft, 0_ft}, {0_ft, 1_ft}, {-2_ft, 2_ft}, {2_ft, 4_ft}, {2_ft, 6_ft}}),
             {.curve = true, .start = 90_deg});
      }
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
