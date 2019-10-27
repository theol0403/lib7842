#include "test.hpp"

int lvglMain();

int main(int argc, char** argv) {
  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running lvgl:" << std::endl;
    lvglMain();
  } else {
    std::cout << "Running gtest:" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
}