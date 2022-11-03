#include "kontomire/core/renderer.h"

namespace Knt
{

std::unique_ptr<RenderAPI> Renderer::render_api_ = RenderAPI::create();

} // namespace Knt
