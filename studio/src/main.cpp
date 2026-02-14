#include "toolbox/base/base.hpp"
#include "toolbox/gfx/gfx.hpp"

using namespace ct;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    log::Configure("toolbox");

    auto window = TRY(Window::Create({.title = "toolbox", .width = 1080, .height = 720}));
    auto device = TRY(Device::Create({.validate = true, .verbose = true}));
    auto surface = TRY(Surface::Create(window, device, {}));


    // auto surface = TRY(Surface::Create( window , device, {}));


    while (!window->ShouldClose()) {
        window->PollEvents();
    }
}
