#ifndef __KONTOMIRE_OPENGL_LAYER__
#define __KONTOMIRE_OPENGL_LAYER__

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <implot/implot.h>

#include "kontomire/core/buffer.h"
#include "kontomire/core/framebuffer.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/core/shader.h"
#include "kontomire/core/vertex_arrays.h"
#include "layer.h"

const std::string vertex_shader_src = "#version 330 core\n"
                                      "layout (location = 0) in vec3 aPos;\n"
                                      "out vec3 vPos;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   vPos = aPos;\n"
                                      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                      "}\0";

const std::string fragment_shader_src = "#version 330 core\n"
                                        "layout(location = 0) out vec4 color;\n"
                                        "uniform vec4 vColor;\n"
                                        "in vec3 vPos;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   color = vec4(vColor.x, vColor.y, vColor.z, 1.0f);\n"
                                        "}\0";

uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

float vertices[3 * 4] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
class OpenGLLayer : public Layer
{
  private:
    glm::vec4 square_color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 background_color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

    std::shared_ptr<Kontomire::Shader> shader{};
    std::shared_ptr<Kontomire::RenderAPI> api{};
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer{};
    std::shared_ptr<Kontomire::VertexArray> vertex_array{};

  public:
    void init() override
    {
        api = Kontomire::RenderAPI::create();
        shader = Kontomire::Shader::create("basic", vertex_shader_src, fragment_shader_src);

        vertex_array = Kontomire::VertexArray::create();

        auto vertex_buffer = Kontomire::VertexBuffer::create(vertices, sizeof(vertices));
        Kontomire::BufferLayout layout = {{Kontomire::ShaderDataType::Float3, "aPos"}};
        vertex_buffer->set_layout(layout);
        vertex_array->add_vertex_buffer(vertex_buffer);

        auto index_buffer = Kontomire::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
        vertex_array->set_index_buffer(index_buffer);

        Kontomire::FramebufferSpecification framebuffer_specs;
        framebuffer_specs.attachments = {Kontomire::FramebufferTextureFormat::RGBA8,
                                         Kontomire::FramebufferTextureFormat::RED_INTEGER,
                                         Kontomire::FramebufferTextureFormat::Depth};
        framebuffer_specs.width = 1280;
        framebuffer_specs.height = 720;
        framebuffer = Kontomire::FrameBuffer::create(framebuffer_specs);
    };

    void draw() const
    {
        framebuffer->bind();

        api->set_clear_color(background_color);
        api->clear();

        shader->bind();
        shader->set_float4("vColor", square_color);

        api->draw_indexed(vertex_array);

        framebuffer->clear_attachment(1, -1);
        framebuffer->unbind();
    }

    void update() const override
    {
        draw();

        uint32_t texture = framebuffer->get_color_attachment_id();

        ImGui::Begin("Kontomire");
        {
            ImGui::BeginChild("Viewport");
            ImGui::Image(reinterpret_cast<void*>(texture), ImGui::GetWindowSize(), ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::EndChild();
        }
        ImGui::End();

        static bool alpha = true;
        static bool alpha_bar = true;
        static bool side_preview = true;
        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int display_mode = 0;
        static int picker_mode = 0;
        ImGui::Checkbox("With Alpha", &alpha);
        ImGui::Checkbox("With Alpha Bar", &alpha_bar);
        ImGui::Checkbox("With Side Preview", &side_preview);

        ImGuiColorEditFlags flags{};
        if (!alpha)
            flags |=
                ImGuiColorEditFlags_NoAlpha; // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar)
            flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview)
            flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1)
            flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)
            flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1)
            flags |= ImGuiColorEditFlags_NoInputs; // Disable all RGB/HSV/Hex displays
        if (display_mode == 2)
            flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
        if (display_mode == 3)
            flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4)
            flags |= ImGuiColorEditFlags_DisplayHex;

        ImGui::Begin("Color Panel");
        {
            ImGui::Text("Color picker:");
            ImGui::BeginGroup();
            ImGui::ColorPicker4("Square Color", (float*)&square_color);
            ImGui::ColorPicker4("Background Color", (float*)&background_color,
                                flags | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview);
            ImGui::EndGroup();
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_OPENGL_LAYER__