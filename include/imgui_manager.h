#ifndef __LAME_GAME_IMGUI_LAYER__
#define __LAME_GAME_IMGUI_LAYER__

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
    void init(GLFWwindow* window) noexcept;
    void set_size(int width, int height) noexcept;
    void render(const std::function<void(void)>& callback) const noexcept;

    ImguiWindowManager(const std::string& glsl_version_str);
    ~ImguiWindowManager();
};

#endif // __LAME_GAME_IMGUI_LAYER__