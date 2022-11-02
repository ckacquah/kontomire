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

class DemoLayer : public Layer
{
  private:
    float yaw{Kontomire::CameraDefaults::YAW};
    float zoom{Kontomire::CameraDefaults::ZOOM};
    float pitch{Kontomire::CameraDefaults::PITCH};

    glm::vec4 square_color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
    glm::vec4 background_color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

    glm::vec2 square_size = glm::vec2(1.0f, 1.0f);
    glm::vec3 square_position = glm::vec3(0.0f, 0.0f, 5.0f);

    glm::vec2 circle_size = glm::vec2(0.725f, 0.75f);
    glm::vec3 circle_position = glm::vec3(-1.0f, 0.0f, 5.0f);

    std::shared_ptr<Kontomire::Texture2D> texture;
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer{};

    Kontomire::Camera camera = Kontomire::Camera();
    Kontomire::FramebufferSpecification framebuffer_specs;

  public:
    void init() override
    {
        framebuffer_specs.attachments = {Kontomire::FramebufferTextureFormat::Depth,
                                         Kontomire::FramebufferTextureFormat::RGBA8,
                                         Kontomire::FramebufferTextureFormat::RED_INTEGER};
        framebuffer_specs.width = 1280;
        framebuffer_specs.height = 720;

        texture = Kontomire::Texture2D::create("assets/textures/texture.jpg");

        framebuffer = Kontomire::FrameBuffer::create(framebuffer_specs);

        camera.set_aspect_ratio(static_cast<float>(framebuffer_specs.width) /
                                static_cast<float>(framebuffer_specs.height));

        Kontomire::Renderer::set_line_width(1.0f);

        Kontomire::Renderer2D::init();
    };

    void draw()
    {
        framebuffer->bind();

        Kontomire::Renderer::set_clear_color(background_color);
        Kontomire::Renderer::clear();

        Kontomire::Renderer2D::begin(camera);
        Kontomire::Renderer2D::draw_quad(square_position, square_size, texture, square_color);
        Kontomire::Renderer2D::draw_line(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 5.0f), square_color);
        Kontomire::Renderer2D::draw_circle(circle_position, circle_size, square_color);
        Kontomire::Renderer2D::end();

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

            ImGui::SliderFloat2("Square size", reinterpret_cast<float*>(&square_size), 0.0f, 1.0f, "%.3f");
            ImGui::SliderFloat3("Square position", reinterpret_cast<float*>(&square_position), -20.0f, 20.0f, "%.3f");

            ImGui::Separator();

            ImGui::SliderFloat2("Circle size", reinterpret_cast<float*>(&circle_size), 0.0f, 1.0f, "%.3f");
            ImGui::SliderFloat3("Circle position", reinterpret_cast<float*>(&circle_position), -20.0f, 20.0f, "%.3f");

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