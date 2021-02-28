#pragma once
#include "generator.hpp"

namespace lib7842 {

class SkidSteerGenerator {
public:
  virtual ~SkidSteerGenerator() = default;

  SkidSteerGenerator(std::shared_ptr<ChassisModel> imodel, const QAngularSpeed& igearset,
                     const ChassisScales& iscales, const Limits& ilimits, const QTime& idt,
                     bool iisXdrive = false) :
    model(std::move(imodel)),
    gearset(igearset),
    scales(iscales),
    limits(ilimits),
    dt(idt),
    isXdrive(iisXdrive) {
    if (isXdrive) { limits.v *= std::sqrt(2); }
  };

  Generator::Output follow(const Spline& spline, bool forward = true,
                           const ProfileFlags& flags = {},
                           const std::vector<std::pair<Number, Number>>& markers = {});

  virtual void executor(const Generator::DriveCommand& c);

protected:
  std::shared_ptr<ChassisModel> model;
  QAngularSpeed gearset;
  ChassisScales scales;
  Limits limits;
  QTime dt;
  bool isXdrive;
};

} // namespace lib7842