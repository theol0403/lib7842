#pragma once
#include "main.h"
#include "simplePath.hpp"

#include "lib7842/point/dataPoint.hpp"

namespace lib7842 {

class DataPath : public SimplePath {

public:
  /**
   * Default Constructors
   */
  DataPath() = default;
  DataPath(const DataPath& ipath) = default;
  virtual ~DataPath() = default;

  explicit DataPath(const std::initializer_list<DataPoint>& ipath);
  explicit DataPath(const std::vector<DataPoint>& ipath);
  explicit DataPath(const std::vector<std::shared_ptr<DataPoint>>& ipath);

  std::vector<std::shared_ptr<DataPoint>>& get();
  std::vector<std::shared_ptr<DataPoint>>& operator()();
  const std::vector<std::shared_ptr<DataPoint>>& read() const;

  /**
   * Interpolate the path
   *
   * @param isteps how many points to interpolate per segment, from start (inclusive) to end (exclusive) of segment
   * @return generated path
   */
  virtual SimplePath generate(const int isteps = 1) const override;

  /**
   * Return shared pointer to copy of path
   */
  virtual std::shared_ptr<AbstractPath> copyPtr() const override;

  /**
   * Move the path into a shared pointer and return pointer
   */
  virtual std::shared_ptr<AbstractPath> movePtr() const override;

protected:
  std::vector<std::shared_ptr<DataPoint>> path {};
};

} // namespace lib7842