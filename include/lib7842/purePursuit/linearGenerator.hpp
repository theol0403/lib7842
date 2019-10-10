#pragma once
#include "main.h"
#include "lib7842/purePursuit/abstractGenerator.hpp"

namespace lib7842 {

class LinearGenerator : public AbstractGenerator {

 public:
  /**
   * Custom Types
   */
  struct smoothParams_t {
    const double weight;
    const QLength tolerance;
  };

  /**
   * Constructors
   */
  LinearGenerator(const AbstractPath& ipath, const smoothParams_t& ismoothParams);
  LinearGenerator(const LinearGenerator& igenerator) = default;
  virtual ~LinearGenerator() = default;

  /**
   * Extractors
   */
  virtual SimplePath generate(const QLength& ispacing) const override;

  /**
   * Custom Members
   */
  static SimplePath insert(const AbstractPath& ipath, const QLength& ispacing);
  static SimplePath smoothen(const AbstractPath& ipath, const smoothParams_t& ismoothParams);

 protected:
  const AbstractPath& path;
  const smoothParams_t smoothParams;
};

} // namespace lib7842
