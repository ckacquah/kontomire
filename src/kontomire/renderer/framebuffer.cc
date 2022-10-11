#include <memory>

#include "kontomire/renderer/framebuffer.h"
#include "kontomire/renderer/renderAPI.h"
#include "platform/opengl/framebuffer.h"

namespace Kontomire {

std::shared_ptr<FrameBuffer> FrameBuffer::create(const FramebufferSpecification& spec)
{
    switch (RenderAPI::get_API()) {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::shared_ptr<FrameBuffer>(new OpenGLFramebuffer(spec));
    }
    return nullptr;
}

} // namespace Kontomire
