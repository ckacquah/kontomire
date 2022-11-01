#include "application.h"

Application::Application() : window(std::make_unique<Window>("Kontomire Playground"))
{
    layers.push_back(std::make_unique<TestLayer>());
    layers.push_back(std::make_unique<DemoLayer>());

    window->create();

    for (const auto& layer : layers)
    {
        layer->init();
    }

    window->set_callback([&]() {
        for (const auto& layer : layers)
        {
            layer->update();
        }
    });
}

void Application::run()
{
    while (!window->is_closed())
    {
        window->update();
    }
}