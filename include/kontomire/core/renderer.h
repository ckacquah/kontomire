#ifndef __KONTOMIRE_RENDERER__
#define __KONTOMIRE_RENDERER__

#include <iostream>
#include <memory>

#include "kontomire/core/renderAPI.h"

namespace Kontomire
{

class Renderer
{
  private:
    static std::unique_ptr<RenderAPI> render_api_;

  public:
    static void init()
    {
        render_api_->init();
    }

    static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        render_api_->set_viewport(x, y, width, height);
    }

    static void set_clear_color(const glm::vec4& color)
    {
        render_api_->set_clear_color(color);
    }

    static void clear()
    {
        render_api_->clear();
    }

    static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0)
    {
        render_api_->draw_indexed(vertex_array, index_count);
    }

    static void draw_lines(const std::shared_ptr<VertexArray>& vertex_array, uint32_t vertex_count)
    {
        render_api_->draw_lines(vertex_array, vertex_count);
    }

    static void set_line_width(float width)
    {
        render_api_->set_line_width(width);
    }
};

} // namespace Kontomire

#endif // __KONTOMIRE_RENDERER__