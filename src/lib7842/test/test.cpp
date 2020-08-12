#define DOCTEST_CONFIG_NO_WINDOWS_SEH
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT
#include "lib7842/test/mocks.hpp"

using namespace test;

void runUnitTests() {
  doctest::Context context;
  context.run(); // run
}