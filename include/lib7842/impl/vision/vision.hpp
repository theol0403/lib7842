#pragma once
#include "lib7842/api/vision/container.hpp"
#include "pros/vision.hpp"
#include <memory>

namespace lib7842::Vision {

class Vision : public pros::Vision {

public:
  explicit Vision(std::uint8_t iport);

  Container getAll();

protected:
  std::uint8_t port;
};

} // namespace lib7842::Vision