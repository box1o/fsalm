#include "toolbox/gfx/api/device.hpp"
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

} // namespace detail

result<ref<Device>> Device::Create(const DeviceInfo& info) {
    ref<Device> dev(new Device());
    if (!dev->CreateInstace()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create instance");
    }

    if (!dev->CreateAdapter()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create adapter");
    }

    if (!dev->CreateDevice()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create device");
    }

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
                // std::cerr << "Failed to request adapter: " << ToString(message) << '\n';
                return;
            }
            log::Info("Adapter requested successfully.");
            adapterState.adapter = adapter;
        });

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
    log::Info("  Vendor: {}", dsv(adapterInfo.vendor));
    log::Info("  Architecture: {}", dsv(adapterInfo.architecture));
    log::Info("  Device: {}", dsv(adapterInfo.device));
    log::Info("  Description: {}", dsv(adapterInfo.description));
    log::Info("  VendorID: 0x{:x}", adapterInfo.vendorID);
    log::Info("  DeviceID: 0x{:x}", adapterInfo.deviceID);
    log::Info("  BackendType: {}", static_cast<u32>(adapterInfo.backendType));
    log::Info("  AdapterType: {}", static_cast<u32>(adapterInfo.adapterType));

    return true;
}

bool Device::CreateDevice() { return true; }

} // namespace ct
