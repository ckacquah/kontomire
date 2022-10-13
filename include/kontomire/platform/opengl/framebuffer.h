#ifndef __OPENGL_CPP_TEMPLATE_OPENGL_FRAMEBUFFER__
#define __OPENGL_CPP_TEMPLATE_OPENGL_FRAMEBUFFER__

#include "kontomire/renderer/framebuffer.h"

namespace Kontomire {

class OpenGLFramebuffer : public FrameBuffer {
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
        return _color_attachment[index];
    }

    virtual const FramebufferSpecification& get_specification() const override { return _specification; }

private:
    uint32_t _id = 0;
    FramebufferSpecification _specification;

    std::vector<FramebufferTextureSpecification> _color_attachment_specification;
    FramebufferTextureSpecification _depth_attachment_specification = FramebufferTextureFormat::None;

    std::vector<uint32_t> _color_attachment;
    uint32_t _depth_attachment = 0;
};

} // namespace Kontomire

#endif // __OPENGL_CPP_TEMPLATE_OPENGL_FRAMEBUFFER__