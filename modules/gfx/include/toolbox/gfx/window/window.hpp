#pragma once

#include <toolbox/base/base.hpp>

#include "GLFW/glfw3.h"

#include <string>

namespace ct {

struct WindowInfo {
    std::string title{"toolbox"};
    u32 width{800};
    u32 height{600};
    bool fullscreen{false};
    bool resizable{true};
    bool decorated{true};
};

class Window {
public:
    virtual ~Window();
    // [[nodiscard]]  void* GetWindowHandle() const noexcept {};
    // [[nodiscard]]  bool ShouldClose() const noexcept {};
    //  void PollEvents() const noexcept {};

    static result<ref<Window>> Create(const WindowInfo& info = {});

protected:
    Window() = default;

private:
    GLFWwindow* mWindowHandle{nullptr};
};

} // namespace ct
