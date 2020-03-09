#pragma once
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "pursuitLimits.hpp"
#include <optional>

namespace lib7842 {

// forward declaration
template <typename T> class PursuitPathHelper;
using PursuitPath = PursuitPathHelper<DataPoint>;
using PursuitStatePath = PursuitPathHelper<DataState>;

/**
 * A Path that contains PursuitLimits.
 */
template <typename T> class PursuitPathHelper : public DiscretePath<T> {
public:
  /**
   * A PursuitPath is treated like a Path
   */
  using DiscretePath<T>::DiscretePath;

  /**
   * Set the limits.
   *
   * @param ilimits The pursuit limits
   */
  void setLimits(const PursuitLimits& ilimits) {
    limits = ilimits;
  }

  /**
   * Get the limits. Will throw an exception if no limits are set.
   *
   * @return The limits.
   */
  PursuitLimits getLimits() const {
    if (limits) {
      return limits.value();
    } else {
      throw std::runtime_error("PursuitPath::getLimits: no limits set");
    }
  }

  /**
   * @return true if limits have been set.
   */
  bool hasLimits() const {
    return limits.has_value();
  }

protected:
  std::optional<PursuitLimits> limits {std::nullopt};
};
} // namespace lib7842
