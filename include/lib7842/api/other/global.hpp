#pragma once
#include "okapi/api/util/logging.hpp"
#include "okapi/api/util/timeUtil.hpp"

namespace lib7842::global {

using namespace okapi;

/**
 * Set a new global logger. lib7842 classes will all use this logger.
 *
 * @param ilogger The new logger instance.
 */
void setLogger(const std::shared_ptr<Logger>& ilogger);

/**
 * Get the global logger.
 *
 * @return The logger.
 */
std::shared_ptr<Logger> getLogger();

/**
 * Set a new global timeUtil. lib7842 classes will all use this timeUtil.
 *
 * @param itimeUtil The new timeUtil
 */
void setTimeUtil(const std::shared_ptr<TimeUtil>& itimeUtil);

/**
 * Get the global timeUtil.
 *
 * @return The timeUtil.
 */
std::shared_ptr<TimeUtil> getTimeUtil();

#define GLOBAL_DEBUG(msg) lib7842::global::getLogger()->debug([=]() { return msg; })
#define GLOBAL_INFO(msg) lib7842::global::getLogger()->info([=]() { return msg; })
#define GLOBAL_WARN(msg) lib7842::global::getLogger()->warn([=]() { return msg; })
#define GLOBAL_ERROR(msg) lib7842::global::getLogger()->error([=]() { return msg; })

#define GLOBAL_DEBUG_S(msg) GLOBAL_DEBUG(std::string(msg))
#define GLOBAL_INFO_S(msg) GLOBAL_INFO(std::string(msg))
#define GLOBAL_WARN_S(msg) GLOBAL_WARN(std::string(msg))
#define GLOBAL_ERROR_S(msg) GLOBAL_ERROR(std::string(msg))

} // namespace lib7842::global
