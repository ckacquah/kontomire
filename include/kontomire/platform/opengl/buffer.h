#ifndef __OPENGL_CPP_TEMPLATE_OPENGL_BUFFER__
#define __OPENGL_CPP_TEMPLATE_OPENGL_BUFFER__

#include <stdint.h>

#include "kontomire/renderer/buffer.h"

namespace Kontomire {

class OpenGLVertexBuffer : public VertexBuffer {
private:
    uint32_t _id {};
    BufferLayout _layout {};

public:
    OpenGLVertexBuffer() {};
    OpenGLVertexBuffer(const float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    void bind() const noexcept override;
    void unbind() const noexcept override;

    const BufferLayout& get_layout() const noexcept override { return _layout; };
    void set_layout(const BufferLayout& layout) noexcept override { _layout = layout; };
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const noexcept override;
    virtual void unbind() const noexcept override;

    virtual uint32_t get_count() const noexcept override { return _count; }

private:
    uint32_t _id;
    uint32_t _count;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_OPENGL_BUFFER__
