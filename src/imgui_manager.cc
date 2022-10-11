#include <array>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "kontomire/core/imgui_manager.h"

ImguiWindowManager::ImguiWindowManager(const std::string& glsl_version_str)
    : glsl_version(glsl_version_str)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
}

ImguiWindowManager::~ImguiWindowManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImguiWindowManager::set_size(int width, int height) noexcept
{
    _width = width;
    _height = height;
};

void ImguiWindowManager::init(GLFWwindow* window) noexcept
{
    window_ptr = window;
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        // style.WindowPadding = ImVec2(0.0f, 0.0f);
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    ImGui_ImplGlfw_InitForOpenGL(window_ptr, true);
}

void ImguiWindowManager::render(const std::function<void(void)>& callback) const noexcept
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    callback();
    ImGui::Render();

    glViewport(0, 0, _height, _width);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}