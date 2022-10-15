
#include <array>
#include <functional>
#include <iostream>
#include <memory>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "imgui_manager.h"
#include "window.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << " " << description << std::endl;
}

Window::Window(const std::string& name, int width, int height) : name(name)
{
    size.width = width;
    size.height = height;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("Couldn't initialize GLFW library");
    }

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    glsl_version = "#version 420";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

    imgui_window_manager = std::make_unique<ImguiWindowManager>(glsl_version);
}

Window::~Window()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Window::is_closed() const
{
    return glfwWindowShouldClose(window);
}

void Window::create()
{
    window = glfwCreateWindow(size.width, size.height, name.c_str(), NULL, NULL);
    if (!window)
    {
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    imgui_window_manager->init(window);
}

void Window::on_update()
{
    glfwPollEvents();
    imgui_window_manager->set_size(size.width, size.height);
    imgui_window_manager->render([=]() { on_update_callback(); });
    glfwSwapBuffers(window);
}

const WindowSize& Window::get_size()
{
    glfwGetFramebufferSize(window, &size.width,
                           &size.height); // update size of window
    return size;
};

void Window::set_callback(const std::function<void(void)>& callback)
{
    on_update_callback = callback;
}