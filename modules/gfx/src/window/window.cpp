#include "toolbox/gfx/window/window.hpp"
#include "toolbox/base/types/types.hpp"

namespace ct {

result<ref<Window>> Window::Create(const WindowInfo& info) {
    ref<Window> win(new Window());




    return win;

}


Window::~Window() {}




} // namespace ct
