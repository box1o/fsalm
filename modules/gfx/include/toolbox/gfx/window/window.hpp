#pragma once
#include <toolbox/base/base.hpp>
#include <functional>
#include <string>
#include <glad/glad.h>  // NOTE: Must be included before GLFW
#include <GLFW/glfw3.h>

namespace ct {

enum class CursorMode : u8 { Normal, Hidden, Disabled };
enum class CursorType : u8 { Arrow, IBeam, Crosshair, Hand, HResize, VResize };

struct WindowInfo {
    std::string title{"Window"};
    u32 width{800};
    u32 height{600};
    bool fullscreen{false};
    bool resizable{true};
    bool decorated{true};
    bool vsync{true};
};


class Window {
public:
    using ResizeCallback = std::function<void(u32 width, u32 height)>;
public:
    explicit Window(const WindowInfo& info);
    ~Window() ;

    [[nodiscard]] const std::string& GetTitle() const noexcept { return info_.title; }
    [[nodiscard]] u32 GetWidth() const noexcept  { return width_; }
    [[nodiscard]] u32 GetHeight() const noexcept  { return height_; }
    [[nodiscard]] f32 GetAspectRatio() const noexcept  { return aspectRatio_; }

    [[nodiscard]] bool IsFullScreen() const noexcept  { return fullscreen_; }
    [[nodiscard]] bool IsVSync() const noexcept  { return vsync_; }

    [[nodiscard]] CursorMode GetCursorMode() const noexcept  { return cursorMode_; }
    void SetCursorMode(CursorMode mode) noexcept ;

    [[nodiscard]] CursorType GetCursorType() const noexcept  { return cursorType_; }
    void SetCursorType(CursorType type) noexcept ;

    [[nodiscard]] f32 GetContentScaleX() const noexcept  { return contentScaleX_; }
    [[nodiscard]] f32 GetContentScaleY() const noexcept  { return contentScaleY_; }

    [[nodiscard]] void* GetNativeWindow() const noexcept  { return handle_; }

    [[nodiscard]] bool ShouldClose() const noexcept ;
    void PollEvents() noexcept ;
    void SwapBuffers() noexcept ;
    void Close() noexcept ;

    void SetResizeCallback(ResizeCallback callback)  { resizeCallback_ = std::move(callback); }

private:
    void InitGLFW();
    void TerminateGLFW();
    void SetupCallbacks();

    WindowInfo info_{};
    u32 width_{0};
    u32 height_{0};
    f32 aspectRatio_{1.0f};
    bool fullscreen_{false};
    bool vsync_{true};

    CursorMode cursorMode_{CursorMode::Normal};
    CursorType cursorType_{CursorType::Arrow};

    f32 contentScaleX_{1.0f};
    f32 contentScaleY_{1.0f};

    GLFWwindow* handle_{nullptr};

    static inline u32 s_windowCount{0};
    static inline bool s_initialized{false};

    ResizeCallback resizeCallback_{};
};

} // namespace cc::gfx
