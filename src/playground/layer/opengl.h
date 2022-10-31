#ifndef __KONTOMIRE_OPENGL_LAYER__
#define __KONTOMIRE_OPENGL_LAYER__

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <implot/implot.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kontomire/core/buffer.h"
#include "kontomire/core/framebuffer.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/core/shader.h"
#include "kontomire/core/texture.h"
#include "kontomire/core/vertex_arrays.h"
#include "layer.h"

const static std::string vertex_shader_src = "#version 330 core\n"
                                             "layout (location = 0) in vec3 a_Pos;\n"
                                             "layout (location = 1) in vec2 a_TexCoord;\n"
                                             "out vec2 v_TexCoord;\n"
                                             "void main()\n"
                                             "{\n"
                                             "   v_TexCoord = a_TexCoord;\n"
                                             "   gl_Position = vec4(a_Pos, 1.0);\n"
                                             "}\n";

const static std::string fragment_shader_src = "#version 330 core\n"
                                               "in vec2 v_TexCoord;\n"
                                               "out vec4 color;\n"
                                               "uniform vec4 u_Color;\n"
                                               "uniform sampler2D u_Texture;\n"
                                               "void main()\n"
                                               "{\n"
                                               "   color = texture(u_Texture, v_TexCoord) * u_Color;\n"
                                               "}\n";
class OpenGLLayer : public Layer
{
  private:
    glm::vec4 square_color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 background_color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

    std::shared_ptr<Kontomire::Shader> shader{};
    std::shared_ptr<Kontomire::RenderAPI> api{};
    std::shared_ptr<Kontomire::Texture2D> texture{};
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer{};
    std::shared_ptr<Kontomire::VertexArray> vertex_array{};

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    float vertices[5 * 4] = {-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,  // top
                             0.75f,  -0.75f, 0.0f, 1.0f, 0.0f,  // left
                             0.75f,  0.75f,  0.0f, 1.0f, 1.0f,  // right
                             -0.75f, 0.75f,  0.0f, 0.0f, 1.0f}; // bottom

  public:
    void init() override
    {
        api = Kontomire::RenderAPI::create();
        shader = Kontomire::Shader::create("basic", vertex_shader_src, fragment_shader_src);
        texture = Kontomire::Texture2D::create("assets/texture.jpg");

        Kontomire::FramebufferSpecification framebuffer_specs;
        framebuffer_specs.attachments = {Kontomire::FramebufferTextureFormat::Depth,
                                         Kontomire::FramebufferTextureFormat::RGBA8,
                                         Kontomire::FramebufferTextureFormat::RED_INTEGER};
        framebuffer_specs.width = 1280;
        framebuffer_specs.height = 720;

        framebuffer = Kontomire::FrameBuffer::create(framebuffer_specs);

        Kontomire::BufferLayout layout = {{Kontomire::ShaderDataType::Float3, "a_Pos"},
                                          {Kontomire::ShaderDataType::Float2, "a_TexCoord"}};

        auto vertex_buffer = Kontomire::VertexBuffer::create(vertices, sizeof(vertices));
        vertex_buffer->set_layout(layout);

        vertex_array = Kontomire::VertexArray::create();
        vertex_array->add_vertex_buffer(vertex_buffer);

        auto index_buffer = Kontomire::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
        vertex_array->set_index_buffer(index_buffer);

        shader->bind();
        shader->set_int("u_Texture", 0);
    };

    void draw() const
    {
        framebuffer->bind();

        api->set_clear_color(background_color);
        api->clear();

        shader->bind();
        texture->bind();
        shader->set_float4("u_Color", square_color);

        api->draw_indexed(vertex_array);

        framebuffer->clear_attachment(1, -1);
        framebuffer->unbind();
    }

    void update() const override
    {
        draw();

        uint32_t texture_id = framebuffer->get_color_attachment_id();

        ImGui::Begin("Kontomire");
        {
            ImGui::BeginChild("Viewport");
            ImGui::Image(reinterpret_cast<void*>(texture_id), ImGui::GetWindowSize(), ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("Settings");
        {
            ImGui::ColorEdit3("Square Color", const_cast<float*>(glm::value_ptr(square_color)));
            ImGui::ColorEdit3("Background Color", const_cast<float*>(glm::value_ptr(background_color)));
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_OPENGL_LAYER__