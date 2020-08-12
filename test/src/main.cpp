#include "lib7842/api/other/global.hpp"
#include "lib7842/test/mocks.hpp"
#include <iostream>
#include <sys/unistd.h>

using namespace test;
int lvglMain();

int main(int argc, char** argv) {
  Logger::setDefaultLogger(
    std::make_shared<Logger>(std::make_unique<MockTimer>(), "/dev/stdout", Logger::LogLevel::warn));

  global::setLogger(
    std::make_shared<Logger>(std::make_unique<MockTimer>(), "/dev/stdout", Logger::LogLevel::off));

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

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

} // namespace pros::c
} // extern "C"