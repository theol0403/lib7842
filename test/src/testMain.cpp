#define DOCTEST_CONFIG_IMPLEMENT
#include "okapi/api/util/logging.hpp"
#include "test.hpp"

int lvglMain();

int main(int argc, char** argv) {
  Logger::setDefaultLogger(std::make_shared<Logger>(std::make_unique<MockTimer>(), "/dev/stdout",
                                                    Logger::LogLevel::debug));

  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running lvgl:" << std::endl;
    return lvglMain();
  } else {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    std::cout << "Running doctest:" << std::endl;
    return context.run(); // run
  }
}