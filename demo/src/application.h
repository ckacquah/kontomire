#ifndef __KONTOMIRE_APPLICATION__
#define __KONTOMIRE_APPLICATION__

#include <memory>
#include <vector>

#include "layer/layer.h"
#include "layer/opengl.h"
#include "layer/test.h"
#include "window.h"

class Application
{
  private:
    std::unique_ptr<Window> window{};
    std::vector<std::unique_ptr<Layer>> layers{};

  public:
    void run();

    Application();
};

Application::Application() : window(std::make_unique<Window>("Kontomire Demo"))
{
    layers.push_back(std::make_unique<TestLayer1>());
    layers.push_back(std::make_unique<OpenGL_Layer>());
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
    while (!window->is_closed())
    {
        window->on_update();
    }
}

#endif // __KONTOMIRE_APPLICATION__