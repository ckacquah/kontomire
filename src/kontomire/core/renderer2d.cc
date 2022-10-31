#include <glm/glm.hpp>

#include "kontomire/core/buffer.h"
#include "kontomire/core/framebuffer.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/core/renderer2d.h"
#include "kontomire/core/shader.h"
#include "kontomire/core/vertex_arrays.h"

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

const uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
const float vertices[3 * 4] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
namespace Kontomire
{

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

void Renderer2D::init()
{
}

void Renderer2D::shutdown()
{
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    auto shader = Kontomire::Shader::create("basic", vertex_shader_src, fragment_shader_src);
    auto vertex_buffer = Kontomire::VertexBuffer::create(vertices, sizeof(vertices));
    auto index_buffer = Kontomire::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

    Kontomire::BufferLayout layout = {{Kontomire::ShaderDataType::Float3, "aPos"}};
    vertex_buffer->set_layout(layout);

    auto vertex_array = Kontomire::VertexArray::create();
    vertex_array->add_vertex_buffer(vertex_buffer);
    vertex_array->set_index_buffer(index_buffer);
}

} // namespace Kontomire