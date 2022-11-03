#include <memory>

#include "kontomire/core/framebuffer.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/platform/opengl/framebuffer.h"

namespace knt
{

std::shared_ptr<FrameBuffer> FrameBuffer::create(const FramebufferSpecification& spec)
{
    switch (RenderAPI::api())
    {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLFramebuffer>(spec);
    }
    return nullptr;
}

} // namespace knt
