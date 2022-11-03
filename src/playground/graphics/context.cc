#include <memory>

#include "kontomire/core/renderAPI.h"

#include "context.h"
#include "opengl/context.h"

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
{
    switch (knt::RenderAPI::api())
    {
    case knt::RenderAPI::API::None:
        return nullptr;
    case knt::RenderAPI::API::OpenGL:
        return std::move(std::make_unique<OpenGLContext>(window));
    }
    return nullptr;
}
