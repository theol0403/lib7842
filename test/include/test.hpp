#include "doctest.h"

#include "implMocks.hpp"

#include "lib7842/api.hpp"
#include "okapi/api.hpp"

using namespace lib7842;
using namespace okapi;
using doctest::Approx;

namespace okapi {
class MockThreeEncoderXDriveModel : public ThreeEncoderXDriveModel {
public:
  MockThreeEncoderXDriveModel();
  std::valarray<std::int32_t> getSensorVals() const override;
  void setSensorVals(std::int32_t left, std::int32_t right, std::int32_t middle);

  std::int32_t leftEnc {0};
  std::int32_t rightEnc {0};
  std::int32_t middleEnc {0};
};

template <typename MassDim, typename LengthDim, typename TimeDim, typename AngleDim>
std::ostream& operator<<(std::ostream& os,
                         const RQuantity<MassDim, LengthDim, TimeDim, AngleDim>& rhs) {
  os << rhs.getValue();
  return os;
}
} // namespace okapi

namespace lib7842 {
std::ostream& operator<<(std::ostream& os, const Vector& rhs);
std::ostream& operator<<(std::ostream& os, const State& rhs);
std::ostream& operator<<(std::ostream& os, const DataPoint& rhs);
} // namespace lib7842
