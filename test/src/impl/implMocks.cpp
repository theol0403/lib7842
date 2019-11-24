#include "test.hpp"

#include "pros/rtos.hpp"

extern "C" {
namespace pros {
namespace c {

void delay(const uint32_t milliseconds) {
  usleep(1000 * milliseconds);
}

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

} // namespace c
} // namespace pros
} // extern "C"
