#ifndef __KONTOMIRE_OPENGL_BUFFER__
#define __KONTOMIRE_OPENGL_BUFFER__

#include <stdint.h>

#include "kontomire/core/buffer.h"

namespace Kontomire
{

class OpenGLVertexBuffer : public VertexBuffer
{
  private:
    uint32_t id_{};
    BufferLayout layout_{};

  public:
    OpenGLVertexBuffer(){};
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    void bind() const override;
    void unbind() const override;

    const BufferLayout& layout() const override
    {
        return layout_;
    };
    void set_layout(const BufferLayout& layout) override
    {
        layout_ = layout;
    };
};

class OpenGLIndexBuffer : public IndexBuffer
{
  public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual uint32_t count() const override
    {
        return count_;
    }

  private:
    uint32_t id_;
    uint32_t count_;
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_BUFFER__
