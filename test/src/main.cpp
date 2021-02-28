#include "lib7842/api.hpp"
#include "lib7842/api/other/global.hpp"
#include "lib7842/test/mocks.hpp"
#include <iostream>
#include <sys/unistd.h>

using namespace test;
int lvglMain();

class SkidSteerGeneratorTest : public SkidSteerGenerator {
public:
  SkidSteerGeneratorTest(const QAngularSpeed& igearset, const ChassisScales& iscales,
                         const Limits& ilimits, const QTime& idt, bool iisXdrive = false) :
    SkidSteerGenerator(nullptr, igearset, iscales, ilimits, idt, iisXdrive) {}

  void executor(const Generator::DriveCommand& c) override {}
};

class XGeneratorTest : public XGenerator {
public:
  XGeneratorTest(const QAngularSpeed& igearset, const ChassisScales& iscales, const Limits& ilimits,
                 const QTime& idt) :
    XGenerator(nullptr, igearset, iscales, ilimits, idt) {}

  void executor(const Generator::DriveCommand& c) override {}
};

int main(int argc, char** argv) {
  global::setTimeUtil(std::make_shared<TimeUtil>(createTimeUtil()));

  if (argc > 1 && std::string(argv[1]) == "lvgl") {
    std::cout << "Running lvgl:" << std::endl;
    return lvglMain();
  }

  ChassisScales scales({3.25_in, 13_in}, 360);
  Limits limits(scales, 200_rpm, 0.6_s);
  SkidSteerGeneratorTest generator(200_rpm, scales, limits, 10_ms, false);

  // auto [t, profile] = generator.follow(
  //   Bezier<3>({{0_ft, 0_ft}, {0_ft, 4_ft}, {2_ft, 0_ft},  {2_ft, 4_ft}}), {.top_v = 100_pct},
  //   {});

  auto [profile, t] =
    generator.follow(Mesh({0_m, 0_m, 0_deg}, {1_ft, 3_ft, 60_deg}), true, {.top_v = 100_pct}, {});

  // auto [t, profile] = generator.follow(Line({0_m, 0_m}, {1_ft, 0_ft}), {.top_v = 100_pct}, {});

  // auto [t, profile] = generator.follow(
  //   make_piecewise<QuinticHermite>({{0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg}, {0_ft, 4_ft,
  //   0_deg}}),
  //   {.top_v = 70_pct});

  if (argc > 1 && std::string(argv[1]) == "print") {
    for (auto&& step : t) {
      std::cout << step.p.x.convert(foot) << "," << step.p.y.convert(foot) << ","
                << step.p.theta.convert(degree) << "," << step.c.convert(1 / meter) << ","
                << step.p_vel << "," << step.k.v << "," << step.w.convert(degree / second) << ","
                << step.left << "," << step.right << std::endl;
    }
  } else {
    std::cout << std::endl;
    std::cout << "Velocity: " << limits.v << " m/s" << std::endl;
    std::cout << "Acceleration: " << limits.a << " m/s2" << std::endl;
    std::cout << "Angular Velocity: " << limits.w.convert(degree / second) << " deg/s" << std::endl;
    std::cout << std::endl;
    std::cout << "Length: " << profile.end().d.convert(foot) << " ft" << std::endl;
    std::cout << "Time: " << profile.end().t << " s" << std::endl;
  }

  // std::cout << "Running benchmark:" << std::endl;
  // for (size_t i = 0; i < 1000; ++i) {
  //   auto t = generator.follow(QuinticHermite({{0_ft, 0_ft, 0_deg}, {2_ft, 2_ft, 0_deg}}));
  // }

  // return runUnitTests(argc, argv);

  return 0;
}

extern "C" {
namespace pros::c {
void delay(const uint32_t milliseconds) { usleep(1000 * milliseconds); }
} // namespace pros::c
} // extern "C"