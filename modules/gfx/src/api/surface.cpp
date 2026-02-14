#include "toolbox/gfx/api/surface.hpp"
#include <webgpu/webgpu_cpp.h>

namespace ct {

namespace detail {} // namespace detail

result<scope<Surface>> Surface::Create(
    weak<Window> window, weak<Device> device, const SurfaceInfo& info) noexcept {
    scope<Surface> surface(new Surface());

    // lock the
    auto win = window.lock();
    if (!win) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to lock window");
    }
    auto dev = device.lock();
    if (!dev) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to lock device");
    }

    if (!surface->CreateSurface(*win, *dev)) {
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to create surface");
    }

    log::Info("[wgpu] Surface created");

    return surface;
}

Surface::~Surface() {}

bool Surface::CreateSurface(const Window& window, const Device& device) { 





    return true; 

}

} // namespace ct
