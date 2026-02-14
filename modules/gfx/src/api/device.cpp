#include "toolbox/gfx/api/device.hpp"
#include "toolbox/gfx/types.hpp"
#include <webgpu/webgpu_cpp.h>

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
#include <cstdlib>
#endif

namespace ct {

namespace detail {

struct RequestAdapterState {
    bool done = false;
    wgpu::Adapter adapter = nullptr;
};

struct RequestDeviceState {
    bool done = false;
    wgpu::Device device = nullptr;
};

} // namespace detail

result<ref<Device>> Device::Create(const DeviceInfo& info) noexcept {
    ref<Device> dev(new Device());
    if (!dev->CreateInstace()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create instance");
    }
    log::Info("[wgpu] Instance created");

    if (!dev->CreateAdapter()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create adapter");
    }
    log::Info("[wgpu] Adapter created\n \t\t Device: {}", dev->GetGPUInfo().device);

    if (!dev->CreateDevice()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create device");
    }
    log::Info("[wgpu] Device created");

    return dev;
}

Device::~Device() {}

bool Device::CreateInstace() {
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    mInstance = wgpu::CreateInstance(nullptr);
#else
    static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    wgpu::InstanceDescriptor desc{
        .requiredFeatureCount = 1,
        .requiredFeatures = &kTimedWaitAny,
    };
    mInstance = wgpu::CreateInstance(&desc);
#endif
    if (mInstance == nullptr) {
        return false;
    }
    return true;
}

// NOTE: Adapter
bool Device::CreateAdapter() {
    wgpu::RequestAdapterOptions options{};
    options.powerPreference = wgpu::PowerPreference::HighPerformance;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    log::Critical("Instance creation for web is not implemented yet.");
    std::abort();

#else
    wgpu::RequestAdapterOptions adapterOpts = {};
    // adapterOpts.compatibleSurface = g.surface;
    adapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;

    detail::RequestAdapterState adapterState;
    mInstance.RequestAdapter(&adapterOpts, wgpu::CallbackMode::AllowProcessEvents,
        [&](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
            adapterState.done = true;
            if (status != wgpu::RequestAdapterStatus::Success) {
                log::Error(
                    "Failed to request adapter: ", std::string(message.data, message.length));
                return;
            }
            log::Info("Adapter requested successfully.");
            adapterState.adapter = adapter;
        });

    // FIXME: This code is blocking , to fix later
    while (!adapterState.done) {
        mInstance.ProcessEvents();
    }

    if (!adapterState.adapter) {
        return false;
    }
    mAdapter = adapterState.adapter;
#endif

    wgpu::AdapterInfo adapterInfo;
    mAdapter.GetInfo(&adapterInfo);

    mAdaperInfo = {
        .vendor = dsv(adapterInfo.vendor),
        .architecture = dsv(adapterInfo.architecture),
        .device = dsv(adapterInfo.device),
        .description = dsv(adapterInfo.description),
        .backendType = static_cast<u32>(adapterInfo.backendType),
        .adapterType = static_cast<u32>(adapterInfo.adapterType),
    };

    return true;
}

bool Device::CreateDevice() {

    wgpu::DeviceDescriptor deviceDesc = {};
    deviceDesc.SetDeviceLostCallback(wgpu::CallbackMode::AllowSpontaneous,
        [](const wgpu::Device&, wgpu::DeviceLostReason reason, wgpu::StringView message) {
            log::Error("Device lost: ", dsv(message));
            return false;
        });
    deviceDesc.SetUncapturedErrorCallback(
        [](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView message) {
            log::Error("WebGPU error (", static_cast<int>(type), "): ", dsv(message));
        });

    detail::RequestDeviceState deviceState;
    mAdapter.RequestDevice(&deviceDesc, wgpu::CallbackMode::AllowProcessEvents,
        [&](wgpu::RequestDeviceStatus status, wgpu::Device device, wgpu::StringView message) {
            deviceState.done = true;
            if (status != wgpu::RequestDeviceStatus::Success) {
                log::Error("Failed to request device: ", dsv(message));
                return;
            }
            deviceState.device = device;
        });

    while (!deviceState.done) {
        mInstance.ProcessEvents();
    }

    if (!deviceState.device) {
        return false;
    }

    mDevice = deviceState.device;
    mQueue = mDevice.GetQueue();

    if (!mDevice) {
        return false;
    }
    if (!mQueue) {
        return false;
    }

    wgpu::Limits limits;
    mDevice.GetLimits(&limits);

    mDeviceCapabilities = {
        .maxVertexAttributes = limits.maxVertexAttributes,
        .maxColorAttachments = limits.maxColorAttachments,
    };

    return true;
}

} // namespace ct
