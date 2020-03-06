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
 * The global logger for use in all lib7842 classes
 */
extern std::shared_ptr<Logger> logger;

/**
 * Set a new global timeUtil. lib7842 classes will all use this timeUtil.
 *
 * @param itimeUtil The new timeUtil
 */
void setTimeUtil(const std::shared_ptr<TimeUtil>& itimeUtil);

/**
 * The global timeUtil for use in all lib7842 classes
 */
extern std::shared_ptr<TimeUtil> timeUtil;

} // namespace lib7842::global