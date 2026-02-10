#include "toolbox/gfx/api/device.hpp"

namespace ct {

ref<Device> Device::Create(const DeviceInfo& info) {
    switch (info.backend) {
        case DeviceBackend::Vulkan:
        case DeviceBackend::Metal:
            log::Critical("Metal backend is not implemented yet");
            std::abort();
        default:
            log::Critical("Unsupported graphics backend");
            std::abort();
    }
}

}
