#ifndef __KONTOMIRE_OPENGL_BUFFER__
#define __KONTOMIRE_OPENGL_BUFFER__

#include <stdint.h>

#include "kontomire/core/buffer.h"

namespace Kontomire {

class OpenGLVertexBuffer : public VertexBuffer {
  private:
    uint32_t _id{};
    BufferLayout _layout{};

  public:
    OpenGLVertexBuffer(){};
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    void bind() const override;
    void unbind() const override;

    const BufferLayout& get_layout() const override { return _layout; };
    void set_layout(const BufferLayout& layout) override { _layout = layout; };
};

class OpenGLIndexBuffer : public IndexBuffer {
  public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t get_count() const override { return _count; }

  private:
    uint32_t _id;
    uint32_t _count;
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_BUFFER__
