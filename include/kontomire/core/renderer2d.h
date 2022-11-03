#ifndef __KONTOMIRE_RENDERER_2D__
#define __KONTOMIRE_RENDERER_2D__

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "kontomire/core/camera.h"
#include "kontomire/core/shader.h"
#include "kontomire/core/texture.h"

namespace knt
{

struct QuadVertex
{
    glm::vec4 color;
    glm::vec3 position;
    glm::vec2 texure_coordinates;

    float texture_index;
};

struct CircleVertex
{
    glm::vec4 color;
    glm::vec3 position;
    glm::vec3 local_position;

    float fade;
    float thickness;
};

struct LineVertex
{
    glm::vec4 color;
    glm::vec3 position;
};

struct Renderer2DData
{
    static const uint32_t MAX_QUADS = 20000;
    static const uint32_t MAX_INDICES = MAX_QUADS * 6;
    static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
    static const uint32_t MAX_TEXTURE_SLOTS = 32;

    std::shared_ptr<Texture2D> white_texture;

    std::shared_ptr<Shader> quad_shader;
    std::shared_ptr<VertexArray> quad_vertex_array;
    std::shared_ptr<VertexBuffer> quad_vertex_buffer;

    std::shared_ptr<Shader> circle_shader;
    std::shared_ptr<VertexArray> circle_vertex_array;
    std::shared_ptr<VertexBuffer> circle_vertex_buffer;

    std::shared_ptr<Shader> line_shader;
    std::shared_ptr<VertexArray> line_vertex_array;
    std::shared_ptr<VertexBuffer> line_vertex_buffer;

    uint32_t quad_index_count = 0;
    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr = nullptr;

    uint32_t circle_index_count = 0;
    CircleVertex* circle_vertex_buffer_base = nullptr;
    CircleVertex* circle_vertex_buffer_ptr = nullptr;

    uint32_t line_vertex_count = 0;
    LineVertex* line_vertex_buffer_base = nullptr;
    LineVertex* line_vertex_buffer_ptr = nullptr;

    uint32_t texture_slots_index = 1;
    std::array<std::shared_ptr<Texture2D>, MAX_TEXTURE_SLOTS> texture_slots;

    glm::vec4 quad_vertex_positions[4];
    glm::mat4 view_projection{};
};

class Renderer2D
{
  public:
    static void init();
    static void flush();
    static void shutdown();

    static void end();
    static void begin(const Camera& camera);

    static void next_batch();
    static void start_batch();

    static void draw_quad(const glm::mat4& transform, const glm::vec4& color);
    static void draw_quad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture,
                          const glm::vec4& color);

    static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture,
                          const glm::vec4& color);

    static void draw_circle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f,
                            float fade = 0.005f);

    static void draw_circle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                            float thickness = 1.0f, float fade = 0.005f);

    static void draw_line(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
};

} // namespace knt

#endif // __KONTOMIRE_RENDERER_2D__
