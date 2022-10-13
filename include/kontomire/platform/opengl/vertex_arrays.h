#ifndef __OPENGL_CPP_TEMPLATE_OPENGL_VERTEX_ARRAYS__
#define __OPENGL_CPP_TEMPLATE_OPENGL_VERTEX_ARRAYS__

#include <memory>

#include "kontomire/renderer/vertex_arrays.h"

namespace Kontomire {

class OpenGLVertexArray : public VertexArray {
private:
    uint32_t _id {};
    uint32_t _vertex_buffer_index {};
    std::vector<std::shared_ptr<VertexBuffer>> _vertex_buffers;
    std::shared_ptr<IndexBuffer> _index_buffer;

public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    // virtual const VertexBuffer& get_vertex_buffer() const noexcept override;
    virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) noexcept override;

    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) noexcept override;
    virtual const std::shared_ptr<IndexBuffer>& get_index_buffer() const noexcept override;

    virtual void bind() const noexcept override;
    virtual void unbind() const noexcept override;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_OPENGL_VERTEX_ARRAYS__
