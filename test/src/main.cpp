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

  for (size_t i = 0; i < 10000; ++i) {
    auto path = QuinticHermite({0_ft, 0_ft, 0_deg}, {1_ft, 1_ft, 0_deg});
    Limits limits(1.5_mps2, 1_mps, 400_deg / second);
    // auto trajectory = TrajectoryGenerator::generate(path, limits, 10_ms);
  }

  return runUnitTests(argc, argv);
  return 0;
}

extern "C" {
namespace pros::c {
void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }
} // namespace pros::c
} // extern "C"