#include <memory>

#include "kontomire/core/renderAPI.h"
#include "kontomire/platform/opengl/vertex_arrays.h"

namespace Kontomire
{

std::shared_ptr<VertexArray> VertexArray::create()
{
    switch (RenderAPI::api())
    {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexArray>();
    }
    return nullptr;
}

} // namespace Kontomire
