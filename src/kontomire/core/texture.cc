#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "kontomire/core/renderAPI.h"
#include "kontomire/core/texture.h"
#include "kontomire/platform/opengl/texture.h"

namespace Kontomire
{

std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
{
    switch (RenderAPI::api())
    {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(width, height);
    }
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::string& path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;

    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        auto texture = Texture2D::create(width, height);
        if (texture)
        {
            texture->set_data(data, width * height * channels, channels);
        }

        stbi_image_free(data);
        return texture;
    }

    return nullptr;
}

} // namespace Kontomire
