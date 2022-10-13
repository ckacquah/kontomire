#include <memory>

#include "kontomire/platform/opengl/vertex_arrays.h"
#include "kontomire/renderer/renderAPI.h"


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
