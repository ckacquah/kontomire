#ifndef __KONTOMIRE_RENDERER_BUFFER__
#define __KONTOMIRE_RENDERER_BUFFER__

#include <functional>
#include <memory>
#include <stdint.h>

namespace knt
{

enum class ShaderDataType
{
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t convert_shader_data_type_to_size(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    }
    return 0;
}

struct BufferElement
{
    std::string name;
    ShaderDataType type;
    uint32_t size;
    size_t offset;
    bool normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : name(name), type(type), size(convert_shader_data_type_to_size(type)), offset(0), normalized(normalized)
    {
    }

    uint32_t get_component_count() const
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3; // 3* float3
        case ShaderDataType::Mat4:
            return 4; // 4* float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        }

        return 0;
    }
};

class BufferLayout
{
  private:
    uint32_t stride_;
    std::vector<BufferElement> elements_;

  public:
    BufferLayout(std::initializer_list<BufferElement> elements) : elements_(elements)
    {
        // Calculate stride and offset
        size_t offset = 0;
        stride_ = 0;
        for (auto& element : elements_)
        {
            element.offset = offset;
            offset += element.size;
            stride_ += element.size;
        }
    }

    uint32_t get_stride() const
    {
        return stride_;
    }

    const std::vector<BufferElement>& elements() const
    {
        return elements_;
    }

    std::vector<BufferElement>::const_iterator end() const
    {
        return elements_.end();
    }

    std::vector<BufferElement>::const_iterator begin() const
    {
        return elements_.begin();
    }
};

class VertexBuffer
{
  public:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_data(const void* data, uint32_t size) = 0;

    virtual void set_layout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& layout() const = 0;

    static std::shared_ptr<VertexBuffer> create(uint32_t size);
    static std::shared_ptr<VertexBuffer> create(const float* vertices, uint32_t size);
};

class IndexBuffer
{
  public:
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t count() const = 0;
    static std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t size);
};

} // namespace knt

#endif // __KONTOMIRE_RENDERER_BUFFER__
