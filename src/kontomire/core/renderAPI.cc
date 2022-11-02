#include "kontomire/core/renderAPI.h"
#include "kontomire/platform/opengl/renderAPI.h"

#include <functional>
#include <memory>

namespace Kontomire
{

RenderAPI::API RenderAPI::api_ = RenderAPI::API::OpenGL;

std::unique_ptr<RenderAPI> RenderAPI::create()
{
    switch (RenderAPI::get_API())
    {
    case RenderAPI::API::None:
    case RenderAPI::API::OpenGL:
        return std::move(std::make_unique<OpenGLRenderAPI>());
    }
    return nullptr;
}

} // namespace Kontomire
