#include <array>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "layer.h"

ImGuiLayer::ImGuiLayer(const GLFWwindow* handle, const std::string& glsl_version)
    : handle_(const_cast<GLFWwindow*>(handle)), version_(glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiLayer::init()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = "assets/config/desktop.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplOpenGL3_Init(version_.c_str());
    ImGui_ImplGlfw_InitForOpenGL(handle_, true);
}

void ImGuiLayer::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void ImGuiLayer::render()
{
    ImGui::Render();

    glViewport(0, 0, height_, width_);
    glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::resize(uint32_t width, uint32_t height)
{
    width_ = width;
    height_ = height;
}