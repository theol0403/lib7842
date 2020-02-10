#include "vision.hpp"
#include "pros/apix.h"

// clang-format off
extern"C"{typedef enum v5_device_e{E_DEVICE_VISION=11}v5_device_e_t;typedef struct _V5_Device*V5_DeviceT;
typedef struct{v5_device_e_t device_type;V5_DeviceT device_info;uint8_t pad[128];}v5_smart_device_s_t;
int32_t registry_validate_binding(uint8_t,v5_device_e_t);v5_smart_device_s_t*registry_get_device(uint8_t);
int port_mutex_take(uint8_t);int port_mutex_give(uint8_t); 
#define claim_port_i(port,device_type)if(registry_validate_binding(port,device_type)!=0)std::cerr<<\
"Unable to validate binding"<<std::endl;v5_smart_device_s_t*device=registry_get_device(port);if(!\
port_mutex_take(port))std::cerr<<"Unable to take port"<<std::endl;
typedef enum{kVisionTypeNormal=0,kVisionTypeColorCode=1,kVisionTypeLineDetect=2}V5VisionBlockType;
typedef struct __attribute((packed))_V5_DeviceVisionObject{uint16_t signature;V5VisionBlockType type;
uint16_t xoffset;uint16_t yoffset;uint16_t width;uint16_t height;uint16_t angle;}V5_DeviceVisionObject;
int32_t vexDeviceVisionObjectCountGet(V5_DeviceT);int32_t vexDeviceVisionObjectGet(V5_DeviceT,uint32_t,
V5_DeviceVisionObject*);
} // clang-format on

namespace lib7842::Vision {

Vision::Vision(std::uint8_t iport) : pros::Vision(iport), port(iport) {}

Container Vision::getAll() {
  Container container;
  claim_port_i(port - 1, E_DEVICE_VISION);

  uint32_t c = vexDeviceVisionObjectCountGet(device->device_info);

  for (uint32_t i = 0; i < c; i++) {
    pros::vision_object_s_t temp;
    vexDeviceVisionObjectGet(device->device_info, i, (V5_DeviceVisionObject*)&temp);
    container.add(Object {temp.signature, (double)temp.left_coord, (double)temp.top_coord,
                          (double)temp.width, (double)temp.height, VISION_FOV_WIDTH,
                          VISION_FOV_HEIGHT});
  }

  port_mutex_give(port - 1);
  return container;
}

} // namespace lib7842::Vision