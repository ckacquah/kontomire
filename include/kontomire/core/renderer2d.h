#ifndef __KONTOMIRE_RENDERER_2D__
#define __KONTOMIRE_RENDERER_2D__

#include <glm/glm.hpp>

namespace Kontomire
{

class Renderer2D
{
  private:
  public:
    static void init();
    static void shutdown();

    static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
};

} // namespace Kontomire

#endif // __KONTOMIRE_RENDERER_2D__
