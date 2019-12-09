#include <doctest.h>

#include "implMocks.hpp"

#include "lib7842/api.hpp"
#include "okapi/api.hpp"

using namespace lib7842;
using namespace okapi;

class MockThreeEncoderXDriveModel : public ThreeEncoderXDriveModel {
public:
  MockThreeEncoderXDriveModel();
  std::valarray<std::int32_t> getSensorVals() const override;
  void setSensorVals(std::int32_t left, std::int32_t right, std::int32_t middle);

  std::int32_t leftEnc {0};
  std::int32_t rightEnc {0};
  std::int32_t middleEnc {0};
};
