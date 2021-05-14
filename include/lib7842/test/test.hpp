#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CONFIG_COLORS_ANSI
#define DOCTEST_CONFIG_NO_WINDOWS_SEH
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS

// #ifndef THREADS_STD
#define DOCTEST_CONFIG_DISABLE
// #endif

#include "lib7842/deps/doctest.h"

namespace lib7842 {}
namespace okapi {}
namespace test {
using namespace lib7842;
using namespace okapi;
using doctest::Approx;

int runUnitTests(int argc, char** argv);
} // namespace test