#include "application.h"

Application::Application() : window_(Window::create("Kontomire Playground", 1280, 768))
{
    imgui_layer_ = std::make_unique<ImGuiLayer>(static_cast<GLFWwindow*>(window_->handle()));
    imgui_layer_->init();

    layers_.push_back(std::make_unique<DemoLayer>());
    layers_.push_back(std::make_unique<ImplotExampleLayer>());

    for (auto& layer : layers_)
    {
        layer->init();
    }
}

void Application::run()
{
    while (!window_->closed())
    {
        auto [width, height] = window_->size();

        imgui_layer_->resize(width, height);
        imgui_layer_->new_frame();

        for (auto& layer : layers_)
        {
            layer->render();
        }

        imgui_layer_->render();
        window_->update();
    }
}