#pragma once
#include "simplePath.hpp"

#include "lib7842/api/positioning/point/dataPoint.hpp"

namespace lib7842 {

class DataPath : public AbstractPath {
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
  explicit DataPath(const SimplePath& ipath);

  std::vector<std::shared_ptr<DataPoint>>& get();
  std::vector<std::shared_ptr<DataPoint>>& operator()();
  const std::vector<std::shared_ptr<DataPoint>>& read() const;

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment
   * @return generated path
   */
  SimplePath generate(const int isteps = 1) const override;

protected:
  std::vector<std::shared_ptr<DataPoint>> path {};
};

} // namespace lib7842