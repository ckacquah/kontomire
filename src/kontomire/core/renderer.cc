#include "kontomire/core/renderer.h"

namespace Kontomire
{

std::unique_ptr<RenderAPI> Renderer::render_api_ = RenderAPI::create();

} // namespace Kontomire
