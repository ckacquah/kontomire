#include "kontomire/platform/opengl/texture.h"

namespace knt
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : width_(width), height_(height), data_format_(GL_RGBA), internal_format_(GL_RGBA8)
{
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &id_);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
    glBindTextureUnit(slot, id_);
}

void OpenGLTexture2D::set_data(void* data, uint32_t size, uint8_t channels)
{
    if (channels == 4)
    {
        data_format_ = GL_RGBA;
        internal_format_ = GL_RGBA8;
    }
    else if (channels == 3)
    {
        data_format_ = GL_RGB;
        internal_format_ = GL_RGB8;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &id_);
    glTextureStorage2D(id_, 1, internal_format_, width_, height_);
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
}

} // namespace knt
