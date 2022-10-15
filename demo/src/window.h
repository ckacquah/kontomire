#ifndef __OPENGL_CPP_TEMPLATE_WINDOWS__
#define __OPENGL_CPP_TEMPLATE_WINDOWS__

#include <memory>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "imgui_manager.h"

struct WindowSize {
    int width {};
    int height {};
};

class Window {
private:
    GLFWwindow* window {};

    WindowSize size {};
    std::string name {};
    std::string glsl_version {};
    std::function<void(void)> on_update_callback {};
    std::unique_ptr<ImguiWindowManager> imgui_window_manager {};

public:
    Window(const std::string& name, int width = 800, int height = 600);
    ~Window();

    void init() const;
    void create();
    bool is_closed() const;
    void on_update();
    void set_callback(const std::function<void(void)>& callback);

    const WindowSize& get_size();
};

#endif // __OPENGL_CPP_TEMPLATE_WINDOWS__