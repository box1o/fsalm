#include "vk_device.hpp"

namespace ct::vk {

VKDeviceImpl::VKDeviceImpl(const DeviceInfo& info) {
    auto rInstance = CreateInstance(info);
    if (!rInstance) {
        log::Error("Failed to create Vulkan instance ");
        return;
    }
    mInstance = *rInstance;

    auto rPhysicalDevice = PickPhysicalDevice(mInstance, info);
    if (!rPhysicalDevice) {
        log::Error("Failed to pick Vulkan physical device");
        return;
    }
    mPhysicalDevice = *rPhysicalDevice;

    auto rDevice = CreateLogicalDevice(info);
    if (!rDevice) {
        log::Error("Failed to create Vulkan logical device");
        return;
    }
    mDevice = *rDevice;
}

VKDeviceImpl::~VKDeviceImpl() {}

std::optional<VkInstance> VKDeviceImpl::CreateInstance(const DeviceInfo& info) {

    return std::nullopt;
}

std::optional<VkPhysicalDevice> VKDeviceImpl::PickPhysicalDevice(
    const VkInstance& instance, const DeviceInfo& info) {

    return std::nullopt;
}

std::optional<VkDevice> VKDeviceImpl::CreateLogicalDevice(const DeviceInfo& info) {

    return std::nullopt;
}

} // namespace ct::vk
