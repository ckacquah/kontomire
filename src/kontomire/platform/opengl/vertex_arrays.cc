#include <cassert>
#include <glad/glad.h>

#include "kontomire/platform/opengl/vertex_arrays.h"

namespace Kontomire {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type) {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    }

    return 0;
}

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
        switch (element.type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: {
            glEnableVertexAttribArray(_vertex_buffer_index);
            glVertexAttribPointer(_vertex_buffer_index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.get_stride(),
                (const void*)element.offset);
            _vertex_buffer_index++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool: {
            glEnableVertexAttribArray(_vertex_buffer_index);
            glVertexAttribIPointer(_vertex_buffer_index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                layout.get_stride(),
                (const void*)element.offset);
            _vertex_buffer_index++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            uint8_t count = element.GetComponentCount();
            for (uint8_t i = 0; i < count; i++) {
                glEnableVertexAttribArray(_vertex_buffer_index);
                glVertexAttribPointer(_vertex_buffer_index,
                    count,
                    ShaderDataTypeToOpenGLBaseType(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    layout.get_stride(),
                    (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(_vertex_buffer_index, 1);
                _vertex_buffer_index++;
            }
            break;
        }
        default:
            assert(false);
        }
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
