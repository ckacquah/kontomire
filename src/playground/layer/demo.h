#ifndef __KONTOMIRE_OPENGL_LAYER__
#define __KONTOMIRE_OPENGL_LAYER__

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <implot/implot.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kontomire.h"
#include "layer.h"

const static std::string vertex_shader_src = "#version 420 core\n"
                                             "layout (location = 0) in vec3 a_Pos;\n"
                                             "layout (location = 1) in vec2 a_TexCoord;\n"
                                             "out vec2 v_TexCoord;\n"
                                             "uniform mat4 u_Model;\n"
                                             "uniform mat4 u_ViewProjection;\n"
                                             "void main()\n"
                                             "{\n"
                                             "   v_TexCoord = a_TexCoord;\n"
                                             "   gl_Position = u_ViewProjection * u_Model * vec4(a_Pos, 1.0);\n"
                                             "}\n";

const static std::string fragment_shader_src = "#version 420 core\n"
                                               "in vec2 v_TexCoord;\n"
                                               "out vec4 color;\n"
                                               "uniform vec4 u_Color;\n"
                                               "uniform sampler2D u_Texture;\n"
                                               "void main()\n"
                                               "{\n"
                                               "   color = texture(u_Texture, v_TexCoord) * u_Color;\n"
                                               "}\n";
class DemoLayer : public Layer
{
  private:
    float yaw{Kontomire::CameraDefaults::YAW};
    float zoom{Kontomire::CameraDefaults::ZOOM};
    float pitch{Kontomire::CameraDefaults::PITCH};

    glm::vec4 square_color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
    glm::vec4 background_color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

    std::shared_ptr<Kontomire::Shader> shader{};
    std::shared_ptr<Kontomire::RenderAPI> api{};
    std::shared_ptr<Kontomire::Texture2D> texture{};
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer{};
    std::shared_ptr<Kontomire::VertexArray> vertex_array{};

    Kontomire::Camera camera = Kontomire::Camera();
    Kontomire::FramebufferSpecification framebuffer_specs;

    glm::mat4 model{glm::mat4(1.0f)};
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
        texture = Kontomire::Texture2D::create("assets/textures/texture.jpg");

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

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
        camera.set_aspect_ratio(static_cast<float>(framebuffer_specs.width) /
                                static_cast<float>(framebuffer_specs.height));
    };

    void draw()
    {
        framebuffer->bind();

        api->set_clear_color(background_color);
        api->clear();

        shader->bind();
        shader->set_float4("u_Color", square_color);
        shader->set_mat4("u_ViewProjection", camera.view_projection());
        shader->set_mat4("u_Model", model);

        texture->bind();
        api->draw_indexed(vertex_array);

        framebuffer->clear_attachment(1, -1);
        framebuffer->unbind();
    }

    void update() override
    {
        draw();

        double interval = (Kontomire::CameraDefaults::SPEED * ImGui::GetFrameCount()) / (ImGui::GetTime() * 3600);

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

            ImGui::Separator();

            ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f, "Yaw = %.3f");
            ImGui::SliderFloat("Pitch", &pitch, -90.0f, 90.0f, "Pitch = %.3f");
            ImGui::Separator();

            camera.rotate(yaw, pitch);

            ImGui::SliderFloat("Zoom", &zoom, 0.0f, 45.0f, "Zoom = %.3f");
            ImGui::Separator();

            camera.zoom(zoom);

            if (ImGui::Button("LEFT"))
            {
                camera.move(Kontomire::Camera::Direction::LEFT, interval);
            }

            ImGui::SameLine();
            if (ImGui::Button("RIGHT"))
            {
                camera.move(Kontomire::Camera::Direction::RIGHT, interval);
            }

            ImGui::SameLine();
            if (ImGui::Button("FORWARD"))
            {
                camera.move(Kontomire::Camera::Direction::FORWARD, interval);
            }

            ImGui::SameLine();
            if (ImGui::Button("BACKWARD"))
            {
                camera.move(Kontomire::Camera::Direction::BACKWARD, interval);
            }
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_OPENGL_LAYER__