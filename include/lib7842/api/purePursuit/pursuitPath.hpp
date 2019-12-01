#pragma once
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "pursuitLimits.hpp"
#include <optional>

namespace lib7842 {

/**
 * A DataPath that contains PursuitLimits.
 */
class PursuitPath : public DataPath {
public:
  /**
   * A PursuitPath is treated like a DataPath
   */
  using DataPath::DataPath;

  /**
   * Set the limits.
   *
   * @param ilimits The pursuit limits
   */
  void setLimits(const PursuitLimits& ilimits);

  /**
   * Get the limits. Will throw an exception if no limits are set.
   *
   * @return The limits.
   */
  PursuitLimits getLimits() const;

  /**
   * @return true if limits have been set.
   */
  bool hasLimits() const;

protected:
  std::optional<PursuitLimits> limits {std::nullopt};
};
} // namespace lib7842