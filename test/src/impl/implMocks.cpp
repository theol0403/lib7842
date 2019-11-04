#include "pros/rtos.hpp"
#include "test.hpp"

#ifdef __cplusplus
extern "C" {
namespace pros {
namespace c {
#endif

void delay(const uint32_t milliseconds) {
  usleep(1000 * milliseconds);
}

void task_delay_until(uint32_t* const prev_time, const uint32_t delta) {
  (void)prev_time;
  usleep(1000 * delta);
}

#ifdef __cplusplus
} // namespace c
} // namespace pros
}
#endif