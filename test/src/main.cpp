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

  ChassisScales scales({3.25_in, 11.3_in}, 360);
  Limits limits(scales, 200_rpm, 1.2_s, std::sqrt(2), 1);
  TestGenerator generator(limits, scales, 10_ms);

  if (argc > 1 && std::string(argv[1]) == "bench") {
    std::cout << "Running benchmark:" << std::endl;
    for (size_t i = 0; i < 10000; ++i) {
      auto path = QuinticHermite({0_ft, 0_ft, 0_deg}, {1_ft, 1_ft, 0_deg});
      auto trajectory = generator.follow(path);
    }
  }

  if (argc > 1 && std::string(argv[1]) == "print") {
    auto t = generator.follow(QuinticHermite({{0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg}}));
    for (auto&& step : t) {
      std::cout << step.p.x.convert(foot) << "," << step.p.y.convert(foot) << ","
                << step.p.theta.convert(degree) << "," << step.c.convert(1 / meter) << ","
                << step.p_vel << "," << step.k.v << "," << step.w.convert(degree / second)
                << std::endl;
    }
  } else {
    return runUnitTests(argc, argv);
  }

  return 0;
}

extern "C" {
namespace pros::c {
void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }
} // namespace pros::c
} // extern "C"