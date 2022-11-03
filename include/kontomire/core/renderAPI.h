#ifndef __KONTOMIRE_RENDERER_API__
#define __KONTOMIRE_RENDERER_API__

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include "vertex_arrays.h"

namespace Knt
{

class RenderAPI
{
  public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

  private:
    static API api_;

  public:
    virtual ~RenderAPI() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void set_line_width(float width) = 0;
    virtual void set_clear_color(const glm::vec4& color) = 0;
    virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void draw_lines(const std::shared_ptr<VertexArray>& vertex_array, uint32_t vertex_count) = 0;
    virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;

    static std::unique_ptr<RenderAPI> create();

    static API api()
    {
        return api_;
    }
};

} // namespace Knt

#endif // __KONTOMIRE_RENDERER_API__
