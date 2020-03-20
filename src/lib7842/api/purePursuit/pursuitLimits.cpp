#include "pursuitLimits.hpp"

namespace lib7842 {

PursuitLimits::PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel,
                             const QSpeed& imaxVel, const QAcceleration& idecel,
                             const QSpeed& ifinalVel, const std::optional<QSpeed>& ik) :
  minVel(iminVel), accel(iaccel), maxVel(imaxVel), decel(idecel), finalVel(ifinalVel), k(ik) {}

PursuitLimits::PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel,
                             const QSpeed& imaxVel, const std::optional<QSpeed>& ik) :
  PursuitLimits(iminVel, iaccel, imaxVel, iaccel, iminVel, ik) {}

PursuitLimits::PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                             const QTime& idecel, const QSpeed& ifinalVel,
                             const std::optional<QSpeed>& ik) :
  PursuitLimits(iminVel, (imaxVel - iminVel) / iaccel, imaxVel, (imaxVel - ifinalVel) / idecel,
                ifinalVel, ik) {}

PursuitLimits::PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                             const std::optional<QSpeed>& ik) :
  PursuitLimits(iminVel, (imaxVel - iminVel) / iaccel, imaxVel, ik) {}

PursuitLimits::PursuitLimits(const QLength& iwheelDiam, const QAngularSpeed& igearset, double imin,
                             const QTime& iaccel, double imax, const QTime& idecel, double ifinal,
                             const std::optional<QSpeed>& ik) :
  PursuitLimits(iwheelDiam * igearset / radian, imin, iaccel, imax, idecel, ifinal, ik) {}

PursuitLimits::PursuitLimits(const QLength& iwheelDiam, const QAngularSpeed& igearset, double imin,
                             const QTime& iaccel, double imax, const std::optional<QSpeed>& ik) :
  PursuitLimits(iwheelDiam, igearset, imin, iaccel, imax, iaccel, imin, ik) {}

PursuitLimits::PursuitLimits(const QSpeed& itopSpeed, double imin, const QTime& iaccel, double imax,
                             const QTime& idecel, double ifinal, const std::optional<QSpeed>& ik) :
  PursuitLimits(itopSpeed * imin, iaccel, itopSpeed * imax, idecel, itopSpeed * ifinal, ik) {}

PursuitLimits::PursuitLimits(const QSpeed& itopSpeed, double imin, const QTime& iaccel, double imax,
                             const std::optional<QSpeed>& ik) :
  PursuitLimits(itopSpeed, imin, iaccel, imax, iaccel, imin, ik) {}

} // namespace lib7842