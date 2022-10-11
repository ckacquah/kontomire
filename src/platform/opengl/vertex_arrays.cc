#include <glad/glad.h>

#include "platform/opengl/vertex_arrays.h"

namespace Kontomire {

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &_id);
}

void OpenGLVertexArray::bind() const noexcept
{
    glBindVertexArray(_id);
}

void OpenGLVertexArray::unbind() const noexcept
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer) noexcept
{
    glBindVertexArray(_id);
    buffer->bind();

    const auto& layout = buffer->get_layout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(_vertex_buffer_index);
        glVertexAttribPointer(_vertex_buffer_index,
            3,
            GL_FLOAT,
            GL_FALSE,
            layout.get_stride(),
            (const void*)element.offset);
        _vertex_buffer_index++;
    }

    _vertex_buffers.push_back(buffer);
}

void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer) noexcept
{
    glBindVertexArray(_id);
    buffer->bind();
    _index_buffer = buffer;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::get_index_buffer() const noexcept
{
    return _index_buffer;
}

} // namespace Kontomire
