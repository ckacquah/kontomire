#include <glad/glad.h>

#include "platform/opengl/buffer.h"

namespace Kontomire {

/********************************/
/*******  Vertex Buffer  ********/
/********************************/

OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
{
    glCreateBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void OpenGLVertexBuffer::bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void OpenGLVertexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/********************************/
/*******  Index Buffer  *********/
/********************************/

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
    : _count(count)
{
    glCreateBuffers(1, &_id);

    // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
    // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &_id);
}

void OpenGLIndexBuffer::bind() const noexcept
{

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void OpenGLIndexBuffer::unbind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Kontomire
