#include "kontomire/core/renderAPI.h"
#include "kontomire/platform/opengl/renderAPI.h"

#include <functional>
#include <memory>

namespace Kontomire
{

std::shared_ptr<RenderAPI> RenderAPI::create()
{
    switch (RenderAPI::get_API())
    {
    case RenderAPI::API::None:
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLRenderAPI>();
    }
    return nullptr;
}

} // namespace Kontomire
