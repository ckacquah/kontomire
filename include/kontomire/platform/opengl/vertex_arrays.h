#ifndef __KONTOMIRE_OPENGL_VERTEX_ARRAYS__
#define __KONTOMIRE_OPENGL_VERTEX_ARRAYS__

#include <memory>

#include "kontomire/core/vertex_arrays.h"

namespace Kontomire {

class OpenGLVertexArray : public VertexArray {
  private:
    uint32_t _id{};
    uint32_t _vertex_buffer_index{};
    std::vector<std::shared_ptr<VertexBuffer>> _vertex_buffers;
    std::shared_ptr<IndexBuffer> _index_buffer;

  public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    // virtual const VertexBuffer& get_vertex_buffer() const override;
    virtual void
    add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) override;

    virtual void
    set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) override;
    virtual const std::shared_ptr<IndexBuffer>&
    get_index_buffer() const override;

    virtual void bind() const override;
    virtual void unbind() const override;
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_VERTEX_ARRAYS__
