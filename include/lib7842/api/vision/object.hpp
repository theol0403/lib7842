#pragma once
#include "query.hpp"
#include <cstdint>

namespace lib7842::Vision {

struct Object {
  double get(const Query& iquery) const;

  uint16_t sig {0};
  double x {0};
  double y {0};
  double width {0};
  double height {0};

  double fovWidth {0};
  double fovHeight {0};
};

} // namespace lib7842::Vision
