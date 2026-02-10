// #include "toolbox/gfx/window/window.hpp"
// #include "../../backends/glfw/glfw_window.hpp"
// #include "toolbox/base/types/types.hpp"
//
// namespace ct {
//
// ref<Window> Window::Create(const WindowInfo& info) {
//     return createRef<GLFWwindowImpl>(info);
//     // TODO:: Implement other backends (Metal, DirectX, etc.)
//     //  switch (info.backend) {
//     //      case DeviceBackend::Vulkan:
//     //          return vk::VKDeviceImpl::Create(info);
//     //      case DeviceBackend::Metal:
//     //          return err( ErrorCode::GRAPHICS_UNSUPPORTED_API, "Metal backend is not implemented
//     //          yet");
//     //      default:
//     //          return err( ErrorCode::GRAPHICS_UNSUPPORTED_API, "Unsupported graphics backend");
//     //  }
// }
//
// } // namespace ct
