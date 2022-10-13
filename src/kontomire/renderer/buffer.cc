#include <memory>

#include "kontomire/platform/opengl/buffer.h"
#include "kontomire/renderer/buffer.h"
#include "kontomire/renderer/renderAPI.h"

namespace Kontomire {

std::shared_ptr<VertexBuffer> VertexBuffer::create(const float* vertices, uint32_t size) noexcept
{
    switch (RenderAPI::get_API()) {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }
    return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices, uint32_t size) noexcept
{
    switch (RenderAPI::get_API()) {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLIndexBuffer>(indices, size);
    }
    return nullptr;
}

} // namespace Kontomire
