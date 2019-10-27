#include "test.hpp"

int main(int argc, char** argv) {
  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running LVGL:" << std::endl;
  } else {
    std::cout << "Running gtest:" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
}