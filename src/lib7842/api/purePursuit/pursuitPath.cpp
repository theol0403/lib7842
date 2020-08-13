#include "lib7842/api/purePursuit/pursuitPath.hpp"

namespace lib7842 {

void PursuitPath::setLimits(const PursuitLimits& ilimits) { limits = ilimits; }

PursuitLimits PursuitPath::getLimits() const {
  if (limits) { return limits.value(); }
  GLOBAL_ERROR_THROW("PursuitPath::getLimits: no limits set");
}

bool PursuitPath::hasLimits() const { return limits.has_value(); }

} // namespace lib7842
