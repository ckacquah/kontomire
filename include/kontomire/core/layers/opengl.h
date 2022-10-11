#ifndef __OPENGL_CPP_TEMPLATE_OPENGL_LAYER__
#define __OPENGL_CPP_TEMPLATE_OPENGL_LAYER__

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <implot/implot.h>

#include "kontomire/core/layer.h"
#include "kontomire/renderer/buffer.h"
#include "kontomire/renderer/framebuffer.h"
#include "kontomire/renderer/renderAPI.h"
#include "kontomire/renderer/vertex_arrays.h"

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const uint32_t indices[] = { 1, 2, 3 };

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

class OpenGL_Layer : public Layer {
private:
    int success {};
    char infoLog[512];
    unsigned int vertexShader {};
    unsigned int fragmentShader {};
    unsigned int shaderProgram {};
    std::shared_ptr<Kontomire::RenderAPI> api {};
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer {};
    std::shared_ptr<Kontomire::VertexArray> vertex_array {};

public:
    ~OpenGL_Layer()
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void init() noexcept override
    {
        api = Kontomire::RenderAPI::create();
        vertex_array = Kontomire::VertexArray::create();
        vertex_array->add_vertex_buffer(Kontomire::VertexBuffer::create(vertices, 9));
        vertex_array->set_index_buffer(Kontomire::IndexBuffer::create(indices, 3));

        Kontomire::FramebufferSpecification fbSpec;
        fbSpec.attachments = { Kontomire::FramebufferTextureFormat::RGBA8, Kontomire::FramebufferTextureFormat::RED_INTEGER, Kontomire::FramebufferTextureFormat::Depth };
        fbSpec.width = 1280;
        fbSpec.height = 720;
        framebuffer = Kontomire::FrameBuffer::create(fbSpec);

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
    };

    void update() const noexcept override
    {
        glUseProgram(shaderProgram);
        framebuffer->bind();
        api->draw_indexed(vertex_array);
        api->set_clear_color(glm::vec4(0.5f, 0.2f, 5.0f, 1.0f));
        api->clear();
        framebuffer->clear_attachment(1, -1);
        framebuffer->unbind();
        uint32_t texture = framebuffer->get_color_attachment_id();

        ImGui::Begin("OpenGL Layer");
        {
            ImGui::Image(reinterpret_cast<void*>(texture), ImGui::GetWindowSize(), ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
            ImGui::End();
        }
    }
};

#endif // __OPENGL_CPP_TEMPLATE_OPENGL_LAYER__