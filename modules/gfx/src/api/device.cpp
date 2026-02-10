#include "toolbox/gfx/api/device.hpp"
#include "../../backends/vulkan/vk_device.hpp"

namespace ct {

ref<Device> Device::Create(const DeviceInfo& info) {
    switch (info.backend) {
        case DeviceBackend::Vulkan:
            return createRef<vk::VKDeviceImpl>(std::move(info));
        case DeviceBackend::Metal:
            log::Critical("Metal backend is not implemented yet");
            std::abort();
        default:
            log::Critical("Unsupported graphics backend");
            std::abort();
    }
}

}
