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
#include "kontomire/core/vertex_arrays.h"
#include "layer.h"

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "out vec3 vPos;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   vPos = aPos;\n"
    "   vColor = aColor;\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "layout(location = 0) out vec4 color;\n"
    "in vec3 vPos;\n"
    "in vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(vColor.x, vColor.y, vColor.z, 1.0f);\n"
    "}\0";

class OpenGL_Layer : public Layer {
  private:
    int success{};
    char infoLog[512];
    unsigned int vertexShader{};
    unsigned int fragmentShader{};
    unsigned int shaderProgram{};
    std::shared_ptr<Kontomire::RenderAPI> api{};
    std::shared_ptr<Kontomire::FrameBuffer> framebuffer{};
    std::shared_ptr<Kontomire::VertexArray> vertex_array{};

  public:
    ~OpenGL_Layer() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void init() noexcept override {
        api = Kontomire::RenderAPI::create();
        vertex_array = Kontomire::VertexArray::create();

        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                                 0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                                 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

        auto vertex_buffer =
            Kontomire::VertexBuffer::create(vertices, sizeof(vertices));
        Kontomire::BufferLayout layout = {
            {Kontomire::ShaderDataType::Float3, "aPos"},
            {Kontomire::ShaderDataType::Float4, "aColor"}};
        vertex_buffer->set_layout(layout);
        vertex_array->add_vertex_buffer(vertex_buffer);

        uint32_t indices[3] = {0, 1, 2};

        auto index_buffer = Kontomire::IndexBuffer::create(
            indices, sizeof(indices) / sizeof(uint32_t));
        vertex_array->set_index_buffer(index_buffer);

        Kontomire::FramebufferSpecification fbSpec;
        fbSpec.attachments = {Kontomire::FramebufferTextureFormat::RGBA8,
                              Kontomire::FramebufferTextureFormat::RED_INTEGER,
                              Kontomire::FramebufferTextureFormat::Depth};
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

    void update() const noexcept override {

        framebuffer->bind();

        api->set_clear_color(glm::vec4(0.5f, 0.2f, 0.5f, 1.0f));

        api->clear();

        framebuffer->clear_attachment(1, -1);

        glUseProgram(shaderProgram);

        api->draw_indexed(vertex_array, 3);

        uint32_t texture = framebuffer->get_color_attachment_id();

        ImGui::Begin("Kontomire");
        {
            ImGui::BeginChild("Viewport");
            ImGui::Image(reinterpret_cast<void*>(texture),
                         ImGui::GetWindowSize(), ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::EndChild();
        }
        ImGui::End();

        framebuffer->unbind();
    }
};

#endif // __KONTOMIRE_OPENGL_LAYER__