#pragma once
#include "toolbox/base/base.hpp"
#include "toolbox/base/types/types.hpp"
#include <webgpu/webgpu_cpp.h>

namespace ct {

enum class NativeSurfaceType : u8 { None, GLFW, Android, Apple, Win32, X11, Wayland };
struct NativeSurfaceHandle {
    NativeSurfaceType type{NativeSurfaceType::None};
    void* window{nullptr};  // GLFWwindow*, ANativeWindow*, NSWindow*, etc.
    void* view{nullptr};    // MTKView*
    void* layer{nullptr};   // CAMetalLayer*
    void* display{nullptr}; // X11 Display*, WL display*, etc.
};

struct SurfaceInfo {};

class Window;
class Device;
class Surface {
public:
    virtual ~Surface();

    [[nodiscard]] static result<scope<Surface>> Create(
        weak<Window> window, weak<Device> device, const SurfaceInfo& info) noexcept;

    // TODO: Implement this latter
    //  [[nodiscard]] static result<scope<Surface>> CreateFromNative(
    //      const NativeSurfaceHandle& handle, weak<Device> device, const SurfaceInfo& info)
    //      noexcept;

    // [[nodiscard]] const NativeSurfaceHandle& GetNativeHandle() const noexcept { return
    // mNativeHandle; };
    //
    //

private:
    Surface() = default;
    bool CreateSurface(const Window& window, const Device& device);

private:
    wgpu::Surface mSurface{nullptr};
};

} // namespace ct
