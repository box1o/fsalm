// #include "toolbox/base/base.hpp"
//
// #include <webgpu/webgpu_cpp.h>
//
// namespace ct {
//
// struct DeviceInfo {
//     bool validate{true};
//     bool verbose{false};
// };
//
// // enum class QueueType : u8 {
// //     Graphics,
// //     Compute,
// //     Transfer,
// //     Present,
// // };
//
// class Device {
// public:
//     virtual ~Device();
//
//     static result<ref<Device>> Create(const DeviceInfo& info);
//
// private:
//     Device() = default;
//
//     bool CreateInstace();
//     bool CreateAdapter();
//     bool CreateDevice();
//
// private:
//     wgpu::Instance mInstance{nullptr};
//     wgpu::Adapter mAdapter{nullptr};
//     wgpu::Device mDevice{nullptr};
// };
//
// } // namespace ct
//
//



// device.hpp
#pragma once

#include "toolbox/base/base.hpp"              // ref<>, result<>, etc.
#include "toolbox/base/errors/errors.hpp"     // ErrorCode, err(...)
#include "toolbox/base/logger/logger.hpp"     // log::Info/Critical
#include "toolbox/base/types/types.hpp"       // u32, etc.

#include <webgpu/webgpu_cpp.h>

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
#include <functional>
#endif

namespace ct {

struct DeviceInfo {
    bool validate{true};                     // enable validation (if supported by backend)
    bool verbose{false};                      // extra logs
    wgpu::PowerPreference powerPreference{wgpu::PowerPreference::HighPerformance};
};

class Device {
public:
    virtual ~Device();

    // Native: synchronous create (WaitAny-based).
    // Web (Emscripten): returns an error; use CreateAsync instead.
    static result<ref<Device>> Create(const DeviceInfo& info);

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    // Web (Emscripten): async create. Calls onDone(result) when ready.
    // Note: this requires you to keep the callback alive until it fires.
    using CreateCallback = std::function<void(result<ref<Device>>)>;

    static void CreateAsync(const DeviceInfo& info, CreateCallback onDone);
#endif

    // Accessors
    const wgpu::Instance& Instance() const { return mInstance; }
    const wgpu::Adapter&  Adapter()  const { return mAdapter;  }
    const wgpu::Device&   WgpuDevice() const { return mDevice; }

    bool IsValid() const { return (mInstance != nullptr) && (mAdapter != nullptr) && (mDevice != nullptr); }

private:
    Device() = default;

    bool CreateInstance();
    bool CreateAdapter(const DeviceInfo& info);
    bool CreateDevice(const DeviceInfo& info);

    void SetupDeviceCallbacks(const DeviceInfo& info);

private:
    wgpu::Instance mInstance{nullptr};
    wgpu::Adapter  mAdapter{nullptr};
    wgpu::Device   mDevice{nullptr};
};

} // namespace ct
