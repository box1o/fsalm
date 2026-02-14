#pragma once
#include "toolbox/base/base.hpp"

#include <webgpu/webgpu_cpp.h>

namespace ct {

struct DeviceInfo {
    bool validate{true};
    bool verbose{false};
};

struct AdapterInfo {
    std::string vendor{};
    std::string architecture{};
    std::string device{};
    std::string description{};
    u32 backendType{};
    u32 adapterType{};
};

struct DeviceCapabilities {
    u32 maxVertexAttributes{0};
    u32 maxColorAttachments{0};
    // TODO: Complete latter with the rest of values i need with data from wgpu::Limits
    // ...
};

class Device {
public:
    virtual ~Device();

    AdapterInfo GetGPUInfo() const noexcept { return mAdaperInfo; };
    DeviceCapabilities GetCapabilities() const noexcept { return mDeviceCapabilities; };

    void* GetNativeDevice() const noexcept { return reinterpret_cast<void*>(mDevice.Get()); };


    [[nodiscard]] static result<ref<Device>> Create(const DeviceInfo& info) noexcept;

private:
    Device() = default;
    bool CreateInstace();
    bool CreateAdapter();
    bool CreateDevice();

private:
    AdapterInfo mAdaperInfo{};

    wgpu::Instance mInstance{nullptr};
    wgpu::Adapter mAdapter{nullptr};
    wgpu::Device mDevice{nullptr};
    wgpu::Queue mQueue{nullptr};

    DeviceCapabilities mDeviceCapabilities{};
};

} // namespace ct
