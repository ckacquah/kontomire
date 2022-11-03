#include <glad/glad.h>

#include "kontomire/platform/opengl/buffer.h"

namespace knt
{

/********************************/
/*******  Vertex Buffer  ********/
/********************************/
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void OpenGLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

void OpenGLVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::set_data(const void* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

/********************************/
/*******  Index Buffer  *********/
/********************************/

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count) : count_(count)
{
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &id_);
}

void OpenGLIndexBuffer::bind() const
{

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void OpenGLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace knt
