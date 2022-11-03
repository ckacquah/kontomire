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

class ImplotExampleLayer : public Layer
{
  private:
    std::vector<int> bar_data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  public:
    ImplotExampleLayer() = default;
    ~ImplotExampleLayer() = default;

    void init() override
    {
    }

    void render() override
    {
        ImGui::Begin("Implot Test");
        if (ImPlot::BeginPlot("My Plot"))
        {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

class DemoLayer : public Layer
{
  private:
    float zoom{};
    double delta{};
    float yaw{knt::CameraDefaults::YAW};
    float pitch{knt::CameraDefaults::PITCH};

    glm::vec4 quad_color{glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)};
    glm::vec4 line_color{glm::vec4(0.2f, 1.0f, 0.0f, 1.0f)};
    glm::vec4 circle_color{glm::vec4(1.0f, 0.0f, 0.5f, 1.0f)};
    glm::vec4 background_color{glm::vec4(0.15f, 0.15f, 0.15f, 1.0f)};

    glm::vec2 square_size{glm::vec2(1.0f, 1.0f)};
    glm::vec2 square_size_prev{glm::vec2(1.0f, 1.0f)};

    glm::vec3 square_position{glm::vec3(0.0f, 0.0f, 1.0f)};
    glm::vec3 square_position_prev{glm::vec3(0.0f, 0.0f, 1.0f)};

    glm::vec2 circle_size{glm::vec2(0.725f, 0.75f)};
    glm::vec3 circle_position{glm::vec3(-1.0f, 0.0f, 1.0f)};

    glm::mat4 transform{glm::mat4(1.0f)};

    std::shared_ptr<knt::Texture2D> texture;
    std::shared_ptr<knt::FrameBuffer> framebuffer{};

    knt::Camera camera{knt::Camera()};
    knt::FramebufferSpecification framebuffer_specs;

  public:
    void init() override
    {
        framebuffer_specs.attachments = {knt::FramebufferTextureFormat::DEPTH, knt::FramebufferTextureFormat::RGBA8,
                                         knt::FramebufferTextureFormat::RED_INTEGER};
        framebuffer_specs.width = 1280;
        framebuffer_specs.height = 720;

        texture = knt::Texture2D::create("assets/textures/texture.jpg");

        framebuffer = knt::FrameBuffer::create(framebuffer_specs);

        camera.set_aspect_ratio(static_cast<float>(framebuffer_specs.width) /
                                static_cast<float>(framebuffer_specs.height));

        knt::Renderer::set_line_width(1.0f);

        knt::Renderer2D::init();

        transform = glm::translate(glm::mat4(1.0f), square_position);
    };

    void draw()
    {
        framebuffer->bind();

        knt::Renderer::set_clear_color(background_color);
        knt::Renderer::clear();

        knt::Renderer2D::begin(camera);

        knt::Renderer2D::draw_line(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 5.0f), line_color);
        // knt::Renderer2D::draw_quad(square_position, square_size, texture, quad_color);

        transform =
            glm::rotate(transform, glm::radians(static_cast<float>(30.0f * delta)), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f});

        if (square_position != square_position_prev)
        {
            transform = glm::translate(glm::mat4(1.0), square_position_prev - square_position) * transform;
        }

        knt::Renderer2D::draw_quad(transform, texture, quad_color);
        knt::Renderer2D::draw_circle(circle_position, circle_size, circle_color);

        knt::Renderer2D::end();

        framebuffer->clear_attachment(1, -1);
        framebuffer->unbind();
    }

    void render() override
    {
        draw();

        square_size_prev = square_size;
        square_position_prev = square_position;

        delta = (knt::CameraDefaults::SPEED * ImGui::GetFrameCount()) / (ImGui::GetTime() * 3600);

        uint32_t texture_id = framebuffer->color_attachment();

        ImGui::Begin("Playground Viewport");
        {
            ImGui::BeginChild("Viewport");
            ImGui::Image(reinterpret_cast<void*>(texture_id), ImGui::GetWindowSize(), ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("Settings");
        {
            ImGui::ColorEdit3("Quad Color", const_cast<float*>(glm::value_ptr(quad_color)));
            ImGui::ColorEdit3("Line Color", const_cast<float*>(glm::value_ptr(line_color)));
            ImGui::ColorEdit3("Circle Color", const_cast<float*>(glm::value_ptr(circle_color)));
            ImGui::ColorEdit3("Background Color", const_cast<float*>(glm::value_ptr(background_color)));

            ImGui::Separator();

            ImGui::SliderFloat2("Square size", reinterpret_cast<float*>(&square_size), 0.0f, 1.0f, "%.3f");
            ImGui::SliderFloat3("Square position", reinterpret_cast<float*>(&square_position), -2.0f, 2.0f, "%.3f");

            ImGui::Separator();

            ImGui::SliderFloat2("Circle size", reinterpret_cast<float*>(&circle_size), 0.0f, 1.0f, "%.3f");
            ImGui::SliderFloat3("Circle position", reinterpret_cast<float*>(&circle_position), -2.0f, 2.0f, "%.3f");

            ImGui::Separator();

            ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f, "%.3f");
            ImGui::SliderFloat("Pitch", &pitch, -90.0f, 90.0f, "%.3f");

            ImGui::Separator();

            camera.rotate(yaw, pitch);

            ImGui::SliderFloat("Zoom", &zoom, 0.0f, 45.0f, "%.3f");
            ImGui::Separator();

            camera.zoom(zoom);

            if (ImGui::Button("LEFT"))
            {
                camera.move(knt::Camera::Direction::LEFT, delta);
            }

            ImGui::SameLine();
            if (ImGui::Button("RIGHT"))
            {
                camera.move(knt::Camera::Direction::RIGHT, delta);
            }

            ImGui::SameLine();
            if (ImGui::Button("FORWARD"))
            {
                camera.move(knt::Camera::Direction::FORWARD, delta);
            }

            ImGui::SameLine();
            if (ImGui::Button("BACKWARD"))
            {
                camera.move(knt::Camera::Direction::BACKWARD, delta);
            }
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_OPENGL_LAYER__