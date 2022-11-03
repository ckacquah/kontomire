#include "kontomire/core/renderer.h"

namespace knt
{

std::unique_ptr<RenderAPI> Renderer::render_api_ = RenderAPI::create();

} // namespace knt
