#ifndef __KONTOMIRE_OPENGL_FRAMEBUFFER__
#define __KONTOMIRE_OPENGL_FRAMEBUFFER__

#include "kontomire/core/framebuffer.h"

namespace Kontomire
{

class OpenGLFramebuffer : public FrameBuffer
{
  private:
    uint32_t id_ = 0;
    uint32_t depth_attachment_ = 0;
    FramebufferSpecification specification_;
    FramebufferTextureSpecification depth_attachment_specification_ = FramebufferTextureFormat::None;

    std::vector<uint32_t> color_attachment_;
    std::vector<FramebufferTextureSpecification> color_attachment_specification_;

  public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    void invalidate();

    virtual void bind() override;
    virtual void unbind() override;

    virtual void resize(uint32_t width, uint32_t height) override;
    virtual int read_pixel(uint32_t attachmentIndex, int x, int y) override;
    virtual void clear_attachment(uint32_t attachmentIndex, int value) override;

    virtual uint32_t get_color_attachment_id(uint32_t index = 0) const override
    {
        return color_attachment_[index];
    }

    virtual const FramebufferSpecification& get_specification() const override
    {
        return specification_;
    }
};

} // namespace Kontomire

#endif // __KONTOMIRE_OPENGL_FRAMEBUFFER__