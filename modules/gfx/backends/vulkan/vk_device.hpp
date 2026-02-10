#pragma once

#include "toolbox/base/errors/result.hpp"
#include "toolbox/gfx/api/device.hpp"

#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace ct::vk {

class VKDeviceImpl final : public Device {
public:
    VKDeviceImpl(const DeviceInfo& info);
    ~VKDeviceImpl() override;

    result<void> Initialize(const DeviceInfo& info); 

    void* GetInstanceHandle() override { return mInstance; };
    void* GetPhysicalDeviceHandle() override { return mPhysicalDevice; };
    void* GetDeviceHandle() override { return mDevice; };

    // void* GetQueueHandle(QueueType type) const override;
    // u32 GetQueueFamilyIndex(QueueType type) const override;

private:
    std::optional<VkInstance> CreateInstance(const DeviceInfo& info);
    std::optional<VkPhysicalDevice> PickPhysicalDevice( const VkInstance& instance, const DeviceInfo& info);
    std::optional<VkDevice> CreateLogicalDevice(const DeviceInfo& info);

private:
    VkDevice mDevice{VK_NULL_HANDLE};
    VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
    VkInstance mInstance{VK_NULL_HANDLE};
};
} // namespace ct::vk
