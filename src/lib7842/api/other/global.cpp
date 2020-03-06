#include "global.hpp"

#if defined(THREADS_STD)
#else
  #include "okapi/impl/util/timeUtilFactory.hpp"
#endif

namespace lib7842::global {

void setLogger(const std::shared_ptr<Logger>& ilogger) {
  logger = ilogger;
}

#if defined(THREADS_STD)
std::shared_ptr<Logger> logger = std::make_shared<Logger>();
#else
std::shared_ptr<Logger> logger =
  std::make_shared<Logger>(std::make_unique<Timer>(), "/ser/sout", Logger::LogLevel::info);
#endif

void setTimeUtil(const std::shared_ptr<TimeUtil>& itimeUtil) {
  timeUtil = itimeUtil;
}

#if defined(THREADS_STD)
std::shared_ptr<TimeUtil> timeUtil = nullptr;
#else
std::shared_ptr<TimeUtil> timeUtil = std::make_shared<TimeUtil>(TimeUtilFactory().create());
#endif

} // namespace lib7842::global