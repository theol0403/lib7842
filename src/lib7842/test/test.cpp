#include "lib7842/api/other/global.hpp"
#define DOCTEST_CONFIG_NO_WINDOWS_SEH
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT
#include "lib7842/test/mocks.hpp"

namespace test {
int runUnitTests(int argc, char** argv) {
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  return context.run(); // run
}
} // namespace test