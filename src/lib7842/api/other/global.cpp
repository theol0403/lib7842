#include "lib7842/api/other/global.hpp"

#if defined(THREADS_STD)
#else
  #include "okapi/impl/util/timeUtilFactory.hpp"
#endif

namespace lib7842::global {

#if defined(THREADS_STD)
static std::shared_ptr<Logger> logger = std::make_shared<Logger>();
#else
static std::shared_ptr<Logger> logger =
  std::make_shared<Logger>(std::make_unique<Timer>(), "/ser/sout", Logger::LogLevel::info);
#endif

void setLogger(const std::shared_ptr<Logger>& ilogger) {
  logger = ilogger;
}

std::shared_ptr<Logger> getLogger() {
  return logger;
}

#if defined(THREADS_STD)
static std::shared_ptr<TimeUtil> timeUtil = nullptr;
#else
static std::shared_ptr<TimeUtil> timeUtil = std::make_shared<TimeUtil>(TimeUtilFactory().create());
#endif

void setTimeUtil(const std::shared_ptr<TimeUtil>& itimeUtil) {
  timeUtil = itimeUtil;
}

std::shared_ptr<TimeUtil> getTimeUtil() {
  return timeUtil;
}

} // namespace lib7842::global
