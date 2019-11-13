#include "test.hpp"

#include "lvgl/lvgl.h"

#include "lib7842/api.hpp"

void lvglTest() {

  Screen scr(lv_scr_act(), LV_COLOR_ORANGE);

  scr.makePage<OdomDebug>().attachOdom(nullptr).attachResetter(nullptr);

  scr.makePage<Graph>().withRange(0, 100).withSeries("Test", LV_COLOR_RED, []() {
    return 50;
  });

  scr.startTask("Screen");

  while (true) {
    pros::delay(100);
  }
}