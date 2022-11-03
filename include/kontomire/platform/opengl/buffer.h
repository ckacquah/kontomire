#ifndef __KONTOMIRE_OPENGL_BUFFER__
#define __KONTOMIRE_OPENGL_BUFFER__

#include <stdint.h>

#include "kontomire/core/buffer.h"

namespace knt
{

class OpenGLVertexBuffer : public VertexBuffer
{
  private:
    uint32_t id_{};
    BufferLayout layout_{};

  public:
    OpenGLVertexBuffer(){};
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    void bind() const override;
    void unbind() const override;

    virtual void set_data(const void* data, uint32_t size) override;

    const BufferLayout& layout() const override
    {
        return layout_;
    }

    void set_layout(const BufferLayout& layout) override
    {
        layout_ = layout;
    }
};

class OpenGLIndexBuffer : public IndexBuffer
{
  private:
    uint32_t id_{};
    uint32_t count_{};

  public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t count() const override
    {
        return count_;
    }
};

} // namespace knt

#endif // __KONTOMIRE_OPENGL_BUFFER__
