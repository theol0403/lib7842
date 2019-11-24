#include "lib7842/api.hpp"
#include "lvgl/lvgl.h"
#include "test.hpp"

void lvglTest() {
  GUI::Screen scr(lv_scr_act(), LV_COLOR_ORANGE);

  scr.makePage<GUI::Odom>().attachOdom(nullptr).attachResetter(nullptr);

  scr.makePage<GUI::Graph>().withRange(0, 100).withSeries("Test", LV_COLOR_RED,
                                                          []() { return 50; });

  scr.makePage<GUI::Actions>("Buttons")
    .button("Claw", [&]() { std::cout << "help" << std::endl; })
    .button("Claw2", [&]() { std::cout << "help" << std::endl; })
    .build();

  scr.makePage<GUI::Selector>("Auton")
    .button("Claw", [&]() { std::cout << "c" << std::endl; })
    .button("Arm", [&]() { std::cout << "a" << std::endl; })
    .button("Yeet", [&]() { std::cout << "y" << std::endl; })
    .build();

  // .button(
  //   "Test",
  //   []() {
  //     std::cout << "Test" << std::endl;
  //   })
  // .button(
  //   "Test2",
  //   []() {
  //     std::cout << "Test" << std::endl;
  //   })
  // .newRow()
  // .button(
  //   "Test3",
  //   []() {
  //     std::cout << "Test" << std::endl;
  //   })
  // .button("Test4", []() {
  //   std::cout << "Test" << std::endl;
  // });

  while (true) {
    pros::delay(100);
  }
}

#include <SDL2/SDL.h>
#include <mutex>
#include <stdlib.h>
#include <unistd.h>

#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/mousewheel.h"

static void hal_init(void);
static int tick_thread(void* data);
static int lvgl_thread(void* data);

int lvglMain() {
  /*Initialize LittlevGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LittlevGL*/
  hal_init();

  lv_theme_set_current(lv_theme_alien_init(40, NULL));

  lvglTest();

  usleep(1000 * 100);

  return 0;
}

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void) {
  /* Add a display
   * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  monitor_init();
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.disp_flush = monitor_flush;
  disp_drv.disp_fill = monitor_fill;
  disp_drv.disp_map = monitor_map;
  lv_disp_drv_register(&disp_drv);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  mouse_init();
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read = mouse_read;
  lv_indev_drv_register(&indev_drv);

  /* Tick init.
   * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
   * Create an SDL thread to do this*/
  SDL_CreateThread(tick_thread, "tick", NULL);
  SDL_CreateThread(lvgl_thread, "lvgl", NULL);
}

std::mutex lvgl_mutex;

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void* data) {
  (void)data;

  while (1) {
    SDL_Delay(10); /*Sleep for 33 millisecond*/
    lvgl_mutex.lock();
    lv_tick_inc(10); /*Tell LittelvGL that 3 milliseconds were elapsed*/
    lvgl_mutex.unlock();
  }

  return 0;
}

static int lvgl_thread(void* data) {
  (void)data;

  while (1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/

    lvgl_mutex.lock();
    lv_task_handler();
    lvgl_mutex.unlock();
    usleep(1000 * 100);
  }

  return 0;
}