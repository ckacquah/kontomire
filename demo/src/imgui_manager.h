#ifndef __OPENGL_CPP_TEMPLATE_IMGUI_LAYER__
#define __OPENGL_CPP_TEMPLATE_IMGUI_LAYER__

#include <array>
#include <functional>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

class ImguiWindowManager {
private:
    ImVec4 clear_color { ImVec4(1.0f, 1.0f, 1.0f, 1.0f) };
    std::string glsl_version {};
    int _width {}, _height {};
    GLFWwindow* window_ptr {};

public:
    void init(GLFWwindow* window);
    void set_size(int width, int height);
    void render(const std::function<void(void)>& callback) const;

    ImguiWindowManager(const std::string& glsl_version_str);
    ~ImguiWindowManager();
};

#endif // __OPENGL_CPP_TEMPLATE_IMGUI_LAYER__