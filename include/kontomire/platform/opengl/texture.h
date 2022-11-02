#ifndef __KONTOMIRE_OPENGL_TEXTURE__
#define __KONTOMIRE_OPENGL_TEXTURE__

#include <glad/glad.h>

#include "kontomire/core/texture.h"

namespace Kontomire
{

class OpenGLTexture2D : public Texture2D
{
  private:
    uint32_t id_{};
    uint32_t width_{};
    uint32_t height_{};

    GLenum data_format_{};
    GLenum internal_format_{};

  public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    virtual ~OpenGLTexture2D();

    virtual uint32_t width() const override
    {
        return width_;
    }

    virtual uint32_t height() const override
    {
        return height_;
    }

    virtual uint32_t id() const override
    {
        return id_;
    }

    virtual bool operator==(const Texture& other) const override
    {
        return id_ == other.id();
    }

    virtual void set_data(void* data, uint32_t size, uint8_t channels) override;

    virtual void bind(uint32_t slot = 0) const override;
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_TEXTURE__
