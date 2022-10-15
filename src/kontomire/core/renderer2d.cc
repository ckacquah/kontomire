#include <glm/glm.hpp>

#include "kontomire/core/renderer2d.h"

namespace Kontomire {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

} // namespace Kontomire