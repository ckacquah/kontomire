#ifndef __KONTOMIRE_VERTEX_ARRAYS__
#define __KONTOMIRE_VERTEX_ARRAYS__

#include "buffer.h"

namespace knt
{

class VertexArray
{
  public:
    virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;
    virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;

    virtual const std::shared_ptr<IndexBuffer>& index_buffer() const = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    static std::shared_ptr<VertexArray> create();
};

} // namespace knt

#endif // __KONTOMIRE_VERTEX_ARRAYS__
