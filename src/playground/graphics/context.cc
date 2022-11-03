#include <memory>

#include "kontomire/core/renderAPI.h"

#include "context.h"
#include "opengl/context.h"

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
{
    switch (Kontomire::RenderAPI::api())
    {
    case Kontomire::RenderAPI::API::None:
        return nullptr;
    case Kontomire::RenderAPI::API::OpenGL:
        return std::move(std::make_unique<OpenGLContext>(window));
    }
    return nullptr;
}
