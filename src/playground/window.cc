#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "graphics/context.h"
#include "window.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << " " << description << std::endl;
}

Window::API Window::api_ = Window::API::Glfw;

std::unique_ptr<Window> Window::create(const std::string& name, uint32_t width, uint32_t height)
{
    switch (Window::api())
    {
    case Window::API::None:
        return nullptr;
    case Window::API::Glfw:
        return std::move(std::make_unique<GlfwWindow>(name, width, height));
    }
    return nullptr;
}

GlfwWindow::GlfwWindow(const std::string& name, uint32_t width, uint32_t height)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("Couldn't initialize GLFW library");
    }

    handle_ = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!handle_)
    {
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1); // Enable vsync

    context_ = GraphicsContext::create(handle_);
    context_->init();
}

GlfwWindow::~GlfwWindow()
{
    destroy();
}

void GlfwWindow::update()
{
    glfwPollEvents();
    context_->swap_buffers();
}

void GlfwWindow::destroy()
{
    if (handle_)
    {
        glfwDestroyWindow(handle_);
        glfwTerminate();

        handle_ = nullptr;
    }
}

bool GlfwWindow::closed() const
{
    return glfwWindowShouldClose(handle_);
}

WindowSize GlfwWindow::size() const
{
    int width{}, height{};
    glfwGetFramebufferSize(handle_, &width, &height);
    return std::make_tuple(width, height);
}
