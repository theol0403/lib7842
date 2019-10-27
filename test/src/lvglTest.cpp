#include "test.hpp"
#include "lvgl/lvgl.h"
#include "main.h"

void lvglTest() {

  lv_obj_t* button = lv_btn_create(lv_scr_act(), NULL);

  while (true) {
    pros::delay(20);
  }
}