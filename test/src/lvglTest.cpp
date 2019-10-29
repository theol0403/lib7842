#include "test.hpp"
#include "main.h"

#include "lib7842/gui/odomDebug.hpp"

void lvglTest() {

  // lv_obj_t* button = lv_btn_create(lv_scr_act(), NULL);

  OdomDebug odom(lv_scr_act(), LV_COLOR_BLUE);
  odom.initialize();

  while (true) {
    pros::delay(20);
  }
}