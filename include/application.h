#ifndef __OPENGL_CPP_TEMPLATE_APPLICATION__
#define __OPENGL_CPP_TEMPLATE_APPLICATION__

#include <memory>
#include <vector>

#include "window.h"

#include "imgui_manager.h"
#include "layer.h"
#include "layers/test.h"

class Application {
private:
    std::unique_ptr<Window> window {};
    std::vector<std::unique_ptr<Layer>> layers {};

public:
    void run();

    Application();
};

Application::Application()
    : window(std::make_unique<Window>("OpenGL CPP Template"))
{
    layers.push_back(std::unique_ptr<Layer>(new TestLayer1()));
    layers.push_back(std::unique_ptr<Layer>(new TestLayer2()));
    layers.push_back(std::unique_ptr<Layer>(new TestLayer3()));
    layers.push_back(std::unique_ptr<Layer>(new TestLayer4()));

    window->create();

    for (const auto& layer : layers)
        layer->init();

    window->set_callback([&]() {
        for (const auto& layer : layers)
            layer->update();
    });
}

void Application::run()
{
    while (!window->is_closed()) {
        window->on_update();
    }
}

#endif // __OPENGL_CPP_TEMPLATE_APPLICATION__