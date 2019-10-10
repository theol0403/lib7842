#pragma once
#include "main.h"
#include "lib7842/purePursuit/abstractGenerator.hpp"

namespace lib7842 {

class LinearGenerator {

 public:
  struct smoothParams_t {
    const double weight;
    const QLength tolerance;
  };

  static SimplePath generate(
    const AbstractPath& ipath, const QLength& ispacing, const smoothParams_t& ismoothParams);

  static SimplePath insert(const AbstractPath& ipath, const QLength& ispacing);
  static SimplePath smoothen(const AbstractPath& ipath, const smoothParams_t& ismoothParams);
};

} // namespace lib7842
