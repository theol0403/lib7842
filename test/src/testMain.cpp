#define DOCTEST_CONFIG_IMPLEMENT
#include "lib7842/api/other/global.hpp"
#include "lib7842/test/mocks.hpp"
#include "okapi/api/util/logging.hpp"

using namespace test;
int lvglMain();

int main(int argc, char** argv) {
  Logger::setDefaultLogger(
    std::make_shared<Logger>(std::make_unique<MockTimer>(), "/dev/stdout", Logger::LogLevel::warn));

  global::setLogger(
    std::make_shared<Logger>(std::make_unique<MockTimer>(), "/dev/stdout", Logger::LogLevel::off));

  global::setTimeUtil(std::make_shared<TimeUtil>(createTimeUtil()));

  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running lvgl:" << std::endl;
    return lvglMain();
  }

  doctest::Context context;
  context.applyCommandLine(argc, argv);
  return context.run(); // run
}