// #include "toolbox/gfx/api/device.hpp"
// #include "toolbox/base/errors/errors.hpp"
// #include "toolbox/base/logger/logger.hpp"
// #include "toolbox/base/types/types.hpp"
// #include <webgpu/webgpu_cpp.h>
//
// #ifdef WEBGPU_BACKEND_EMSCRIPTEN
// #include <cstdlib>
// #endif
//
// namespace ct {
//
// result<ref<Device>> Device::Create(const DeviceInfo& info) {
//     ref<Device> dev(new Device());
//     if (!dev->CreateInstace()) {
//         return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create instance");
//     }
//
//     if (!dev->CreateAdapter()) {
//         return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create adapter");
//     }
//
//     if (!dev->CreateDevice()) {
//         return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create device");
//     }
//
//     return dev;
// }
//
// Device::~Device() {}
//
// bool Device::CreateInstace() {
// #ifdef WEBGPU_BACKEND_EMSCRIPTEN
//     mInstance = wgpu::CreateInstance(nullptr);
// #else
//     static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
//     wgpu::InstanceDescriptor desc{
//         .requiredFeatureCount = 1,
//         .requiredFeatures = &kTimedWaitAny,
//     };
//     mInstance = wgpu::CreateInstance(&desc);
// #endif
//     if (mInstance == nullptr) {
//         return false;
//     }
//     return true;
// }
//
// bool Device::CreateAdapter() {
//     wgpu::RequestAdapterOptions options{};
//     options.powerPreference = wgpu::PowerPreference::HighPerformance;
//
// #ifdef WEBGPU_BACKEND_EMSCRIPTEN
//     log::Critical("Instance creation for web is not implemented yet.");
//     std::abort();
//
// #else
//     auto future = mInstance.RequestAdapter(&options, wgpu::CallbackMode::WaitAnyOnly,
//         [&](wgpu::RequestAdapterStatus status, wgpu::Adapter a, wgpu::StringView message) {
//             if (status != wgpu::RequestAdapterStatus::Success) {
//                 log::Critical("RequestAdapter failed: ", std::string(message.data, message.length));
//                 return;
//             }
//             mAdapter = a;
//         });
//
//     mInstance.WaitAny(future, UINT64_MAX);
//
//     if (mAdapter == nullptr) {
//         log::Critical("RequestAdapter returned no adapter.");
//         return false;
//     }
// #endif
//
//
//     wgpu::AdapterInfo adapterInfo;
//     mAdapter.GetInfo(&adapterInfo);
//     log::Info("Adapter Info:");
//     log::Info("  Vendor: ", std::string(adapterInfo.vendor.data, adapterInfo.vendor.length));
//     log::Info("  Architecture: ", std::string(adapterInfo.architecture.data, adapterInfo.architecture.length));
//     log::Info("  Device: ", std::string(adapterInfo.device.data, adapterInfo.device.length)); log::Info("  Description: ", std::string(adapterInfo.description.data, adapterInfo.description.length));
//     log::Info("  VendorID: 0x", std::hex, adapterInfo.vendorID, std::dec);
//     log::Info("  DeviceID: 0x", std::hex, adapterInfo.deviceID, std::dec);
//     log::Info("  BackendType: ", static_cast<u32>(adapterInfo.backendType));
//     log::Info("  AdapterType: ", static_cast<u32>(adapterInfo.adapterType));
//
//     return true;
// }
//
// bool Device::CreateDevice() { return true; }
//
// } // namespace ct
//
//




// device.cpp
#include "toolbox/gfx/api/device.hpp"

#include <string>
#include <limits>

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
#include <cstdlib>
#endif

namespace ct {

static std::string ToString(wgpu::StringView sv) {
    if (sv.data == nullptr || sv.length == 0) return {};
    return std::string(sv.data, sv.length);
}

Device::~Device() = default;

result<ref<Device>> Device::Create(const DeviceInfo& info) {
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    // Emscripten WebGPU adapter/device requests are inherently async.
    // Provide an explicit async API instead of pretending it's synchronous.
    return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE,
               "Device::Create is synchronous and not supported on Emscripten. Use Device::CreateAsync.");
#else
    ref<Device> dev(new Device());

    if (!dev->CreateInstance()) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create WebGPU instance");
    }

    if (!dev->CreateAdapter(info)) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to acquire WebGPU adapter");
    }

    if (!dev->CreateDevice(info)) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create WebGPU device");
    }

    return dev;
#endif
}

bool Device::CreateInstance() {
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    // Emscripten: CreateInstance(nullptr) is typical.
    mInstance = wgpu::CreateInstance(nullptr);
#else
    // Native path: request TimedWaitAny so we can WaitAny on futures.
    static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;

    wgpu::InstanceDescriptor desc{};
    desc.requiredFeatureCount = 1;
    desc.requiredFeatures = &kTimedWaitAny;

    mInstance = wgpu::CreateInstance(&desc);
#endif

    if (mInstance == nullptr) {
        log::Critical("wgpu::CreateInstance returned null.");
        return false;
    }
    return true;
}

bool Device::CreateAdapter(const DeviceInfo& info) {
    wgpu::RequestAdapterOptions options{};
    options.powerPreference = info.powerPreference;

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    // On web, use CreateAsync() instead (this sync path is not supported).
    (void)options;
    log::Critical("CreateAdapter(sync) called on Emscripten. Use Device::CreateAsync instead.");
    return false;
#else
    auto future = mInstance.RequestAdapter(
        &options,
        wgpu::CallbackMode::WaitAnyOnly,
        [this](wgpu::RequestAdapterStatus status, wgpu::Adapter a, wgpu::StringView message) {
            if (status != wgpu::RequestAdapterStatus::Success) {
                log::Critical("RequestAdapter failed: ", ToString(message));
                return;
            }
            mAdapter = a;
        });

    // Wait forever (or choose a timeout if you prefer).
    mInstance.WaitAny(future, std::numeric_limits<uint64_t>::max());

    if (mAdapter == nullptr) {
        log::Critical("RequestAdapter returned no adapter.");
        return false;
    }

    // Optional adapter info logging
    wgpu::AdapterInfo adapterInfo{};
    mAdapter.GetInfo(&adapterInfo);


    auto sv = [](wgpu::StringView v) -> std::string {
        return (v.data && v.length) ? std::string(v.data, v.length) : std::string("<empty>");
    };

    log::Info("Adapter Info:");
    log::Info("  Vendor: ", sv(adapterInfo.vendor));
    log::Info("  Architecture: ", sv(adapterInfo.architecture));
    log::Info("  Device: ", sv(adapterInfo.device));
    log::Info("  Description: ", sv(adapterInfo.description));

    log::Info("  VendorID: ", std::to_string(adapterInfo.vendorID));
    log::Info("  DeviceID: ", std::to_string(adapterInfo.deviceID));
    log::Info("  BackendType: ", std::to_string((uint32_t)adapterInfo.backendType));
    log::Info("  AdapterType: ", std::to_string((uint32_t)adapterInfo.adapterType));

    if (info.verbose) {
        // You can add feature/limits dumping here if desired.
    }

    return true;
#endif
}

bool Device::CreateDevice(const DeviceInfo& info) {
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
    log::Critical("CreateDevice(sync) called on Emscripten. Use Device::CreateAsync instead.");
    return false;
#else
    wgpu::DeviceDescriptor desc{};

    auto future = mAdapter.RequestDevice(
        &desc,
        wgpu::CallbackMode::WaitAnyOnly,
        [this](wgpu::RequestDeviceStatus status, wgpu::Device d, wgpu::StringView message) {
            if (status != wgpu::RequestDeviceStatus::Success) {
                log::Critical("RequestDevice failed: ", ToString(message));
                return;
            }
            mDevice = d;
        });

    mInstance.WaitAny(future, std::numeric_limits<uint64_t>::max());

    if (mDevice == nullptr) {
        log::Critical("RequestDevice returned no device.");
        return false;
    }

    SetupDeviceCallbacks(info);
    return true;
#endif
}

void Device::SetupDeviceCallbacks(const DeviceInfo& info) {
    if (mDevice == nullptr) return;

    // Uncaptured error callback helps catch validation/runtime errors.
//     mDevice.SetLoggingCallback(a)(
//         [verbose = info.verbose](wgpu::ErrorType type, wgpu::StringView message) {
//             const std::string msg = ToString(message);
//
//             switch (type) {
//                 case wgpu::ErrorType::Validation:
//                     log::Critical("WebGPU validation error: ", msg);
//                     break;
//                 case wgpu::ErrorType::OutOfMemory:
//                     log::Critical("WebGPU out-of-memory error: ", msg);
//                     break;
//                 // case wgpu::ErrorType::DeviceLost:
//                 //     log::Critical("WebGPU device lost: ", msg);
//                 //     break;
//                 case wgpu::ErrorType::Unknown:
//                 default:
//                     log::Critical("WebGPU unknown error: ", msg);
//                     break;
//             }
//
//             if (verbose) {
//                 // Add extra diagnostics if you have them.
//             }
//         });
}

#ifdef WEBGPU_BACKEND_EMSCRIPTEN

void Device::CreateAsync(const DeviceInfo& info, CreateCallback onDone) {
    ref<Device> dev(new Device());

    if (!dev->CreateInstance()) {
        onDone(err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create WebGPU instance"));
        return;
    }

    wgpu::RequestAdapterOptions options{};
    options.powerPreference = info.powerPreference;

    // Request adapter (async)
    dev->mInstance.RequestAdapter(
        &options,
        wgpu::CallbackMode::AllowProcessEvents,
        [dev, info, onDone](wgpu::RequestAdapterStatus status, wgpu::Adapter a, wgpu::StringView message) mutable {
            if (status != wgpu::RequestAdapterStatus::Success || a == nullptr) {
                onDone(err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE,
                           std::string("RequestAdapter failed: ") + ToString(message)));
                return;
            }

            dev->mAdapter = a;

            // Request device (async)
            wgpu::DeviceDescriptor desc{};
            dev->mAdapter.RequestDevice(
                &desc,
                wgpu::CallbackMode::AllowProcessEvents,
                [dev, info, onDone](wgpu::RequestDeviceStatus dStatus, wgpu::Device d, wgpu::StringView dMessage) mutable {
                    if (dStatus != wgpu::RequestDeviceStatus::Success || d == nullptr) {
                        onDone(err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE,
                                   std::string("RequestDevice failed: ") + ToString(dMessage)));
                        return;
                    }

                    dev->mDevice = d;
                    dev->SetupDeviceCallbacks(info);

                    onDone(dev);
                });
        });
}

#endif // WEBGPU_BACKEND_EMSCRIPTEN

} // namespace ct
