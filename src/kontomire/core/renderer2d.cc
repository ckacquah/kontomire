#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "kontomire/core/buffer.h"
#include "kontomire/core/framebuffer.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/core/renderer.h"
#include "kontomire/core/renderer2d.h"
#include "kontomire/core/shader.h"
#include "kontomire/core/texture.h"
#include "kontomire/core/vertex_arrays.h"

namespace knt
{

constexpr uint32_t QUADS_VERTEX_COUNT = 4;
constexpr glm::vec2 TEXTURE_COORDINATES[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

static Renderer2DData render_data_;

void Renderer2D::init()
{
    render_data_.quad_vertex_array = VertexArray::create();

    render_data_.quad_vertex_buffer = VertexBuffer::create(render_data_.MAX_VERTICES * sizeof(QuadVertex));
    render_data_.quad_vertex_buffer->set_layout({
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float2, "a_TexCoord"},
        {ShaderDataType::Float, "a_TexIndex"},
    });

    render_data_.quad_vertex_buffer_base = new QuadVertex[render_data_.MAX_VERTICES];

    uint32_t* quad_indices = new uint32_t[render_data_.MAX_INDICES];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < render_data_.MAX_INDICES; i += 6)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += 4;
    }

    auto index_buffer = IndexBuffer::create(quad_indices, render_data_.MAX_INDICES);

    render_data_.quad_vertex_array->add_vertex_buffer(render_data_.quad_vertex_buffer);
    render_data_.quad_vertex_array->set_index_buffer(index_buffer);

    delete[] quad_indices;

    render_data_.circle_vertex_array = VertexArray::create();

    render_data_.circle_vertex_buffer_base = new CircleVertex[render_data_.MAX_VERTICES];

    render_data_.circle_vertex_buffer = VertexBuffer::create(render_data_.MAX_VERTICES * sizeof(CircleVertex));
    render_data_.circle_vertex_buffer->set_layout({
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float3, "a_WorldPosition"},
        {ShaderDataType::Float3, "a_LocalPosition"},
        {ShaderDataType::Float, "a_Fade"},
        {ShaderDataType::Float, "a_Thickness"},
    });

    render_data_.circle_vertex_array->add_vertex_buffer(render_data_.circle_vertex_buffer);
    render_data_.circle_vertex_array->set_index_buffer(index_buffer);

    render_data_.circle_vertex_buffer_base = new CircleVertex[render_data_.MAX_VERTICES];

    render_data_.line_vertex_array = VertexArray::create();

    render_data_.line_vertex_buffer = VertexBuffer::create(render_data_.MAX_VERTICES * sizeof(CircleVertex));
    render_data_.line_vertex_buffer->set_layout({
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float3, "a_Position"},
    });

    render_data_.line_vertex_buffer_base = new LineVertex[render_data_.MAX_VERTICES];
    render_data_.line_vertex_array->add_vertex_buffer(render_data_.line_vertex_buffer);

    render_data_.white_texture = Texture2D::create(1, 1);

    uint32_t whiteTextureData = 0xffffffff;
    render_data_.white_texture->set_data(&whiteTextureData, sizeof(uint32_t), 3);

    int32_t samplers[render_data_.MAX_TEXTURE_SLOTS];
    for (uint32_t i = 0; i < render_data_.MAX_TEXTURE_SLOTS; i++)
    {
        samplers[i] = i;
    }

    // Set first texture slot to 0
    render_data_.texture_slots[0] = render_data_.white_texture;

    render_data_.quad_shader = Shader::create("assets/shaders/render2d/quad.glsl");
    render_data_.line_shader = Shader::create("assets/shaders/render2d/line.glsl");
    render_data_.circle_shader = Shader::create("assets/shaders/render2d/circle.glsl");

    render_data_.quad_shader->bind();
    render_data_.quad_shader->set_int_array("u_Textures", samplers, render_data_.MAX_TEXTURE_SLOTS);
    render_data_.quad_shader->unbind();

    render_data_.quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    render_data_.quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    render_data_.quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    render_data_.quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    Renderer2D::draw_quad(transform, color);
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,
                           const glm::vec4& color)
{
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    Renderer2D::draw_quad(transform, texture, color);
}

void Renderer2D::draw_circle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness,
                             float fade)
{
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    Renderer2D::draw_circle(transform, color, thickness, fade);
}

void Renderer2D::draw_quad(const glm::mat4& transform, const glm::vec4& color)
{
    const float texture_index = 0.0f; // White Texture

    if (render_data_.quad_index_count >= Renderer2DData::MAX_INDICES)
        next_batch();

    for (size_t i = 0; i < QUADS_VERTEX_COUNT; i++)
    {
        render_data_.quad_vertex_buffer_ptr->color = color;
        render_data_.quad_vertex_buffer_ptr->texture_index = texture_index;
        render_data_.quad_vertex_buffer_ptr->texure_coordinates = TEXTURE_COORDINATES[i];
        render_data_.quad_vertex_buffer_ptr->position = transform * render_data_.quad_vertex_positions[i];
        render_data_.quad_vertex_buffer_ptr++;
    }

    render_data_.quad_index_count += 6;
}

void Renderer2D::draw_quad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture,
                           const glm::vec4& color)
{

    if (render_data_.quad_index_count >= Renderer2DData::MAX_INDICES)
    {
        Renderer2D::next_batch();
    }

    float texture_index = 0.0f;
    for (uint32_t i = 1; i < render_data_.texture_slots_index; i++)
    {
        if (*render_data_.texture_slots[i] == *texture)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        if (render_data_.texture_slots_index >= Renderer2DData::MAX_TEXTURE_SLOTS)
        {
            Renderer2D::next_batch();
        }

        texture_index = (float)render_data_.texture_slots_index;
        render_data_.texture_slots[render_data_.texture_slots_index] = texture;
        render_data_.texture_slots_index++;
    }

    for (size_t i = 0; i < QUADS_VERTEX_COUNT; i++)
    {
        render_data_.quad_vertex_buffer_ptr->color = color;
        render_data_.quad_vertex_buffer_ptr->texture_index = texture_index;
        render_data_.quad_vertex_buffer_ptr->texure_coordinates = TEXTURE_COORDINATES[i];
        render_data_.quad_vertex_buffer_ptr->position = transform * render_data_.quad_vertex_positions[i];
        render_data_.quad_vertex_buffer_ptr++;
    }

    render_data_.quad_index_count += 6;
}

void Renderer2D::draw_circle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
{
    for (size_t i = 0; i < QUADS_VERTEX_COUNT; i++)
    {
        render_data_.circle_vertex_buffer_ptr->fade = fade;
        render_data_.circle_vertex_buffer_ptr->color = color;
        render_data_.circle_vertex_buffer_ptr->thickness = thickness;
        render_data_.circle_vertex_buffer_ptr->position = transform * render_data_.quad_vertex_positions[i];
        render_data_.circle_vertex_buffer_ptr->local_position = render_data_.quad_vertex_positions[i] * 2.0f;
        render_data_.circle_vertex_buffer_ptr++;
    }

    render_data_.circle_index_count += 6;
}

void Renderer2D::draw_line(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
    render_data_.line_vertex_buffer_ptr->position = start;
    render_data_.line_vertex_buffer_ptr->color = color;
    render_data_.line_vertex_buffer_ptr++;

    render_data_.line_vertex_buffer_ptr->position = end;
    render_data_.line_vertex_buffer_ptr->color = color;
    render_data_.line_vertex_buffer_ptr++;

    render_data_.line_vertex_count += 2;
}

void Renderer2D::start_batch()
{
    render_data_.quad_index_count = 0;
    render_data_.quad_vertex_buffer_ptr = render_data_.quad_vertex_buffer_base;

    render_data_.circle_index_count = 0;
    render_data_.circle_vertex_buffer_ptr = render_data_.circle_vertex_buffer_base;

    render_data_.line_vertex_count = 0;
    render_data_.line_vertex_buffer_ptr = render_data_.line_vertex_buffer_base;

    render_data_.texture_slots_index = 1;
}

void Renderer2D::flush()
{
    if (render_data_.quad_index_count)
    {
        uint32_t data_size =
            (uint32_t)((uint8_t*)render_data_.quad_vertex_buffer_ptr - (uint8_t*)render_data_.quad_vertex_buffer_base);
        render_data_.quad_vertex_buffer->set_data(render_data_.quad_vertex_buffer_base, data_size);

        // Bind textures
        for (uint32_t i = 0; i < render_data_.texture_slots_index; i++)
            render_data_.texture_slots[i]->bind(i);

        render_data_.quad_shader->bind();
        Renderer::draw_indexed(render_data_.quad_vertex_array, render_data_.quad_index_count);
    }

    if (render_data_.circle_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)render_data_.circle_vertex_buffer_ptr -
                                        (uint8_t*)render_data_.circle_vertex_buffer_base);
        render_data_.circle_vertex_buffer->set_data(render_data_.circle_vertex_buffer_base, data_size);

        render_data_.circle_shader->bind();
        Renderer::draw_indexed(render_data_.circle_vertex_array, render_data_.circle_index_count);
    }

    if (render_data_.line_vertex_count)
    {
        uint32_t data_size =
            (uint32_t)((uint8_t*)render_data_.line_vertex_buffer_ptr - (uint8_t*)render_data_.line_vertex_buffer_base);
        render_data_.line_vertex_buffer->set_data(render_data_.line_vertex_buffer_base, data_size);

        render_data_.line_shader->bind();
        Renderer::draw_lines(render_data_.line_vertex_array, render_data_.line_vertex_count);
    }
}

void Renderer2D::next_batch()
{
    Renderer2D::flush();
    Renderer2D::start_batch();
}

void Renderer2D::end()
{
    Renderer2D::flush();
}

void Renderer2D::begin(const Camera& camera)
{
    Renderer2D::start_batch();

    render_data_.view_projection = camera.view_projection();

    render_data_.quad_shader->bind();
    render_data_.quad_shader->set_mat4("u_ViewProjection", render_data_.view_projection);

    render_data_.circle_shader->bind();
    render_data_.circle_shader->set_mat4("u_ViewProjection", render_data_.view_projection);
    render_data_.quad_shader->bind();

    render_data_.line_shader->bind();
    render_data_.line_shader->set_mat4("u_ViewProjection", render_data_.view_projection);
}

} // namespace knt