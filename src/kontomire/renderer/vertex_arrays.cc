#include <memory>

#include "kontomire/renderer/renderAPI.h"
#include "platform/opengl/vertex_arrays.h"

namespace Kontomire {

std::shared_ptr<VertexArray> VertexArray::create() noexcept
{
    switch (RenderAPI::get_API()) {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexArray>();
    }
    return nullptr;
}

} // namespace Kontomire
