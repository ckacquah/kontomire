#include <memory>

#include "kontomire/core/renderAPI.h"

#include "context.h"
#include "opengl/context.h"

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
{
    switch (Knt::RenderAPI::api())
    {
    case Knt::RenderAPI::API::None:
        return nullptr;
    case Knt::RenderAPI::API::OpenGL:
        return std::move(std::make_unique<OpenGLContext>(window));
    }
    return nullptr;
}
