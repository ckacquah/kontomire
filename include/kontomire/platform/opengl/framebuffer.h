#ifndef __KONTOMIRE_OPENGL_FRAMEBUFFER__
#define __KONTOMIRE_OPENGL_FRAMEBUFFER__

#include "kontomire/core/framebuffer.h"

namespace Knt
{

class OpenGLFramebuffer : public FrameBuffer
{
  private:
    uint32_t id_{};

    FramebufferSpecification spec_{};

    uint32_t depth_attachment_{};
    FramebufferTextureSpecification depth_attachment_spec_{FramebufferTextureFormat::NONE};

    std::vector<uint32_t> color_attachment_;
    std::vector<FramebufferTextureSpecification> color_attachment_spec_;

  public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    void invalidate();

    virtual void bind() override;
    virtual void unbind() override;

    virtual void resize(uint32_t width, uint32_t height) override;
    virtual void clear_attachment(uint32_t index, int value) override;

    virtual int pixel(uint32_t index, int x, int y) override;

    virtual uint32_t color_attachment(uint32_t index = 0) const override
    {
        return color_attachment_[index];
    }

    virtual const FramebufferSpecification& specification() const override
    {
        return spec_;
    }
};

} // namespace Knt

#endif // __KONTOMIRE_OPENGL_FRAMEBUFFER__