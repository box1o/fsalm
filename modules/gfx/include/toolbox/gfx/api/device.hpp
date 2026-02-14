#pragma once
#include "toolbox/base/base.hpp"
#include "toolbox/gfx/types.hpp"

#include <webgpu/webgpu_cpp.h>


namespace ct {

struct DeviceInfo {
    bool validate{true};
    bool verbose{false};
};


class Device {
public:
    virtual ~Device();

    static result<ref<Device>> Create(const DeviceInfo& info);

private:
    Device() = default;

    bool CreateInstace();
    bool CreateAdapter();
    bool CreateDevice();

private:
    wgpu::Instance mInstance{nullptr};
    wgpu::Adapter mAdapter{nullptr};
    wgpu::Device mDevice{nullptr};
};

} // namespace ct


