#ifndef __KONTOMIRE_OPENGL_RENDER_API__
#define __KONTOMIRE_OPENGL_RENDER_API__

#include "kontomire/core/renderAPI.h"

namespace Kontomire {

class OpenGLRenderAPI : public RenderAPI {
  private:
  public:
    virtual void init() override;
    virtual void clear() override;
    virtual void set_clear_color(const glm::vec4& color) override;
    virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                              uint32_t height) override;
    virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array,
                              uint32_t index_count = 0) override;
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_RENDER_API__
