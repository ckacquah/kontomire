#include "kontomire/platform/opengl/vertex_arrays.h"

#include <glad/glad.h>

#include <cassert>

namespace Kontomire
{

static GLenum convert_to_opengl_base_type(ShaderDataType type)
{
    switch (type)
    {
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
    glCreateVertexArrays(1, &id_);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &id_);
}

void OpenGLVertexArray::bind() const
{
    glBindVertexArray(id_);
}

void OpenGLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& buffer)
{
    glBindVertexArray(id_);
    buffer->bind();

    const auto& layout = buffer->layout();
    for (const auto& element : layout)
    {
        switch (element.type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: {
            glEnableVertexAttribArray(vertex_buffer_index_);
            glVertexAttribPointer(vertex_buffer_index_, element.get_component_count(),
                                  convert_to_opengl_base_type(element.type), element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.get_stride(), (const void*)element.offset);
            vertex_buffer_index_++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool: {
            glEnableVertexAttribArray(vertex_buffer_index_);
            glVertexAttribIPointer(vertex_buffer_index_, element.get_component_count(),
                                   convert_to_opengl_base_type(element.type), layout.get_stride(),
                                   (const void*)element.offset);
            vertex_buffer_index_++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            uint8_t count = element.get_component_count();
            for (uint8_t i = 0; i < count; i++)
            {
                glEnableVertexAttribArray(vertex_buffer_index_);
                glVertexAttribPointer(vertex_buffer_index_, count, convert_to_opengl_base_type(element.type),
                                      element.normalized ? GL_TRUE : GL_FALSE, layout.get_stride(),
                                      (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(vertex_buffer_index_, 1);
                vertex_buffer_index_++;
            }
            break;
        }
        default:
            assert(false);
        }
    }

    vertex_buffers_.push_back(buffer);
}

void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& buffer)
{
    glBindVertexArray(id_);
    buffer->bind();
    index_buffer_ = buffer;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::index_buffer() const
{
    return index_buffer_;
}

} // namespace Kontomire
