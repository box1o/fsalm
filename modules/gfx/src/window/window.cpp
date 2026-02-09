#include "toolbox/gfx/window/window.hpp"

namespace ct {

namespace {
constexpr int OPENGL_MAJOR_VERSION = 4;
constexpr int OPENGL_MINOR_VERSION = 6;

void ErrorCallback(int error, const char* description) {
    log::Error("GLFW Error ({}): {}", error, description ? description : "(null)");
}

[[nodiscard]] GLFWcursor* CreateGlfwCursor(CursorType type) noexcept {
    switch (type) {
        case CursorType::Arrow:     return glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        case CursorType::IBeam:     return glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        case CursorType::Crosshair: return glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        case CursorType::Hand:      return glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        case CursorType::HResize:   return glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        case CursorType::VResize:   return glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        default:                    return nullptr;
    }
}
} // namespace

Window::Window(const WindowInfo& info)
    : info_(info) {
    InitGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, info.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, info.decorated ? GLFW_TRUE : GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWmonitor* monitor = info.fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    handle_ = glfwCreateWindow(
        static_cast<int>(info.width),
        static_cast<int>(info.height),
        info.title.c_str(),
        monitor,
        nullptr
    );

    if (!handle_) {
        if (s_windowCount == 0) TerminateGLFW();
        log::Critical("Failed to create GLFW window");
        std::abort();
    }

    ++s_windowCount;

    width_ = info.width;
    height_ = info.height;
    fullscreen_ = info.fullscreen;
    vsync_ = info.vsync;
    aspectRatio_ = (height_ > 0) ? static_cast<f32>(width_) / static_cast<f32>(height_) : 1.0f;

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(vsync_ ? 1 : 0);

    float sx = 1.0f, sy = 1.0f;
    glfwGetWindowContentScale(handle_, &sx, &sy);
    contentScaleX_ = sx;
    contentScaleY_ = sy;

    SetupCallbacks();
}

Window::~Window() {
    if (handle_) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;

        if (s_windowCount > 0) --s_windowCount;
        if (s_windowCount == 0) TerminateGLFW();
    }
}

void Window::SetupCallbacks() {
    glfwSetWindowUserPointer(handle_, this);

    glfwSetFramebufferSizeCallback(handle_, [](GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (!self) return;

        self->width_ = static_cast<u32>(width);
        self->height_ = static_cast<u32>(height);
        self->aspectRatio_ = (self->height_ > 0)
            ? static_cast<f32>(self->width_) / static_cast<f32>(self->height_)
            : 1.0f;

        if (self->resizeCallback_) self->resizeCallback_(self->width_, self->height_);
    });

    glfwSetWindowContentScaleCallback(handle_, [](GLFWwindow* window, float sx, float sy) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (!self) return;
        self->contentScaleX_ = sx;
        self->contentScaleY_ = sy;
    });
}

void Window::SetCursorMode(CursorMode mode) noexcept {
    cursorMode_ = mode;
    int value = GLFW_CURSOR_NORMAL;
    switch (mode) {
        case CursorMode::Normal:   value = GLFW_CURSOR_NORMAL; break;
        case CursorMode::Hidden:   value = GLFW_CURSOR_HIDDEN; break;
        case CursorMode::Disabled: value = GLFW_CURSOR_DISABLED; break;
    }
    glfwSetInputMode(handle_, GLFW_CURSOR, value);
}

void Window::SetCursorType(CursorType type) noexcept {
    GLFWcursor* cursor = CreateGlfwCursor(type);
    if (!cursor) return;
    glfwSetCursor(handle_, cursor);
    cursorType_ = type;
}

bool Window::ShouldClose() const noexcept {
    return !handle_ || glfwWindowShouldClose(handle_) != 0;
}

void Window::PollEvents() noexcept { glfwPollEvents(); }

void Window::SwapBuffers() noexcept {
    if (handle_) glfwSwapBuffers(handle_);
}

void Window::Close() noexcept {
    if (handle_) glfwSetWindowShouldClose(handle_, GLFW_TRUE);
}

void Window::InitGLFW() {
    if (s_initialized) return;

    glfwSetErrorCallback(ErrorCallback);

    if (glfwInit() == 0) {
        log::Critical("Failed to initialize GLFW");
        std::abort();
    }

    s_initialized = true;
}

void Window::TerminateGLFW() {
    if (!s_initialized) return;
    glfwTerminate();
    s_initialized = false;
}

} // namespace cc::gfx
