#pragma once
#include "generator.hpp"

namespace lib7842 {

class XGenerator {
public:
  virtual ~XGenerator() = default;

  XGenerator(std::shared_ptr<XDriveModel> imodel, const QAngularSpeed& igearset,
             const ChassisScales& iscales, const Limits& ilimits, const QTime& idt) :
    model(std::move(imodel)), gearset(igearset), scales(iscales), limits(ilimits), dt(idt) {
    limits.v *= std::sqrt(2);
  };

  Generator::Output follow(const Spline& spline, const ProfileFlags& flags = {},
                           const PiecewiseTrapezoidal::Markers& markers = {});

protected:
  std::shared_ptr<XDriveModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
};

} // namespace lib7842