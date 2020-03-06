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

} // namespace lib7842::global