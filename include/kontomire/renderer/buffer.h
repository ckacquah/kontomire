#ifndef __OPENGL_CPP_TEMPLATE_RENDERER_BUFFER__
#define __OPENGL_CPP_TEMPLATE_RENDERER_BUFFER__

#include <functional>
#include <memory>
#include <stdint.h>

namespace Kontomire {

struct BufferElement {
    std::string name;
    uint32_t size;
    size_t offset;

    BufferElement(const std::string& name, uint32_t size, size_t offset)
        : name(name)
        , size(size)
        , offset(offset)
    {
    }
};

class BufferLayout {
private:
    uint32_t _stride;
    std::vector<BufferElement> _elements;

public:
    BufferLayout(std::initializer_list<BufferElement> elements)
        : _elements(elements)
    {
        // Calculate stride and offset
        size_t offset = 0;
        _stride = 0;
        for (auto& element : _elements) {
            element.offset = offset;
            offset += element.size;
            _stride += element.size;
        }
    }

    uint32_t get_stride() const { return _stride; }
    const std::vector<BufferElement>& get_elements() const { return _elements; };

    std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
};

class VertexBuffer {
public:
    virtual void bind() const noexcept = 0;
    virtual void unbind() const noexcept = 0;

    virtual void set_layout(const BufferLayout& layout) noexcept = 0;
    virtual const BufferLayout& get_layout() const noexcept = 0;

    static std::shared_ptr<VertexBuffer> create(const float* vertices, uint32_t size) noexcept;
};

class IndexBuffer {
public:
    virtual void bind() const noexcept = 0;
    virtual void unbind() const noexcept = 0;

    virtual uint32_t get_count() const noexcept = 0;
    static std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t size) noexcept;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_RENDERER_BUFFER__
