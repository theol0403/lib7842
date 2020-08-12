// #include "implMocks.hpp"
#include "pros/rtos.hpp"
#include <unistd.h>

#define protected public
#include "lib7842/test/mocks.hpp"
using namespace test;

extern "C" {
namespace pros::c {

void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

} // namespace pros::c
} // extern "C"
