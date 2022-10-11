#ifndef __OPENGL_CPP_TEMPLATE_VERTEX_ARRAYS__
#define __OPENGL_CPP_TEMPLATE_VERTEX_ARRAYS__

#include "kontomire/renderer/buffer.h"

namespace Kontomire {

class VertexArray {
public:
    virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) noexcept = 0;

    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) noexcept = 0;
    virtual const std::shared_ptr<IndexBuffer>& get_index_buffer() const noexcept = 0;

    virtual void bind() const noexcept = 0;
    virtual void unbind() const noexcept = 0;

    static std::shared_ptr<VertexArray> create() noexcept;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_VERTEX_ARRAYS__
