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

  return runUnitTests(argc, argv);
}

extern "C" {
namespace pros::c {
void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }
} // namespace pros::c
} // extern "C"