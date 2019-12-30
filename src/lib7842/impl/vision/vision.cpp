#include "vision.hpp"
#include "pros/apix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum v5_device_e {
  E_DEVICE_NONE = 0,
  E_DEVICE_MOTOR = 2,
  E_DEVICE_RADIO = 8,
  E_DEVICE_VISION = 11,
  E_DEVICE_ADI = 12,
  E_DEVICE_GENERIC = 129,
  E_DEVICE_UNDEFINED = 255
} v5_device_e_t;

typedef struct _V5_Device* V5_DeviceT;

typedef struct {
  v5_device_e_t device_type;
  V5_DeviceT device_info;
  uint8_t pad[128]; // 16 bytes in adi_data_s_t times 8 ADI Ports = 128
} v5_smart_device_s_t;

int32_t registry_validate_binding(uint8_t port, v5_device_e_t expected_t);
v5_smart_device_s_t* registry_get_device(uint8_t port);
int port_mutex_take(uint8_t port);
int port_mutex_give(uint8_t port);

#define claim_port_i(port, device_type)                                                            \
  if (registry_validate_binding(port, device_type) != 0)                                           \
    std::cerr << "Unable to validate binding" << std::endl;                                        \
  v5_smart_device_s_t* device = registry_get_device(port);                                         \
  if (!port_mutex_take(port)) std::cerr << "Unable to take port" << std::endl;

typedef enum {
  kVisionTypeNormal = 0,
  kVisionTypeColorCode = 1,
  kVisionTypeLineDetect = 2
} V5VisionBlockType;

typedef struct __attribute((packed)) _V5_DeviceVisionObject {
  uint16_t signature; /// block signature
  V5VisionBlockType type; /// block type
  uint16_t xoffset; /// left side of block
  uint16_t yoffset; /// top of block
  uint16_t width; /// width of block
  uint16_t height; /// height of block
  uint16_t angle; /// angle of CC block in 0.1 deg units
} V5_DeviceVisionObject;

int32_t vexDeviceVisionObjectCountGet(V5_DeviceT device);
int32_t vexDeviceVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                 V5_DeviceVisionObject* pObject);

#ifdef __cplusplus
}
#endif

namespace lib7842::Vision {

Vision::Vision(std::uint8_t iport) : pros::Vision(iport), port(iport) {}

Container Vision::getAll() {
  Container container;
  claim_port_i(port - 1, E_DEVICE_VISION);

  uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);

  for (uint32_t i = 0; i < c; i++) {
    pros::vision_object_s_t temp;
    vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)&temp);
    container.add(Object(temp));
  }

  port_mutex_give(port - 1);
  return container;
}

} // namespace lib7842::Vision