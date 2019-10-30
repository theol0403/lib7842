#include "test.hpp"
#include "main.h"

#include "lib7842/gui/odomDebug.hpp"

void lvglTest() {

  // lv_obj_t* button = lv_btn_create(lv_scr_act(), NULL);


  while (true) {
    debug.render();
    pros::delay(100);
  }
}