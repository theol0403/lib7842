#include "test.hpp"

#include "lvgl/lvgl.h"

#include "lib7842/api.hpp"

void lvglTest() {

  Screen scr(lv_scr_act(), LV_COLOR_ORANGE);
  scr.startTask("Screen");

  scr.makePage<OdomDebug>().attachOdom(nullptr).attachResetter(nullptr);

  scr.makePage<Graph>().withRange(0, 100).withSeries("Test", LV_COLOR_RED, []() {
    return 50;
  });

  scr.makePage<ButtonMatrix>("Buttons")
    .makeButton(
      "Test",
      []() {
        std::cout << "Test" << std::endl;
      })
    .makeButton(
      "Test2",
      []() {
        std::cout << "Test" << std::endl;
      })
    .makeBreak()
    .makeButton(
      "Test3",
      []() {
        std::cout << "Test" << std::endl;
      })
    .makeButton(
      "Test4",
      []() {
        std::cout << "Test" << std::endl;
      })
    .build();

  while (true) {
    pros::delay(100);
  }
}