#ifndef __OPENGL_CPP_TEMPLATE_RENDERER_API__
#define __OPENGL_CPP_TEMPLATE_RENDERER_API__

#include <memory>
#include <stdint.h>

#include <glm/glm.hpp>

#include "kontomire/renderer/vertex_arrays.h"

namespace Kontomire {

class RenderAPI {
public:
    enum class API {
        None = 0,
        OpenGL = 1
    };

private:
    static const API _api { API::OpenGL };

public:
    virtual ~RenderAPI() = default;

    virtual void init() noexcept = 0;
    virtual void clear() noexcept = 0;
    virtual void set_clear_color(const glm::vec4& color) noexcept = 0;
    virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept = 0;

    virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;

    static API get_API() noexcept { return _api; }
    static std::shared_ptr<RenderAPI> create() noexcept;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_RENDERER_API__
