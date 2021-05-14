#include "lib7842/api/other/global.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "lib7842/test/mocks.hpp"

#ifndef DOCTEST_CONFIG_DISABLE
namespace test {
int runUnitTests(int argc, char** argv) {
  global::setLogger(
    std::make_shared<Logger>(std::make_unique<MockTimer>(), "/ser/sout", Logger::LogLevel::off));

  doctest::Context context;
  context.applyCommandLine(argc, argv);
  return context.run(); // run
}
} // namespace test
#endif