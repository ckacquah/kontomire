#ifndef __KONTOMIRE_OPENGL_VERTEX_ARRAYS__
#define __KONTOMIRE_OPENGL_VERTEX_ARRAYS__

#include <memory>

#include "kontomire/core/vertex_arrays.h"

namespace Knt
{

class OpenGLVertexArray : public VertexArray
{
  private:
    uint32_t id_{};
    uint32_t vertex_buffer_index_{};

    std::shared_ptr<IndexBuffer> index_buffer_;
    std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;

  public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    // virtual const VertexBuffer& get_vertex_buffer() const override;
    virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) override;

    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) override;
    virtual const std::shared_ptr<IndexBuffer>& index_buffer() const override;

    virtual void bind() const override;
    virtual void unbind() const override;
};

} // namespace Knt

#endif // __KONTOMIRE_OPENGL_VERTEX_ARRAYS__
