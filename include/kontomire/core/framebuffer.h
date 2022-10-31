#ifndef __KONTOMIRE_RENDERER_FRAMEBUFFER__
#define __KONTOMIRE_RENDERER_FRAMEBUFFER__

#include <memory>
#include <stdint.h>
#include <vector>

namespace Kontomire
{

enum class FramebufferTextureFormat
{
    None = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format) : texture_format(format)
    {
    }

    FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : attachments(attachments)
    {
    }

    std::vector<FramebufferTextureSpecification> attachments;
};

struct FramebufferSpecification
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t samples = 1;

    bool swap_chain_target = false;
    FramebufferAttachmentSpecification attachments;
};

class FrameBuffer
{
  public:
    virtual ~FrameBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;
    virtual int read_pixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual const FramebufferSpecification& get_specification() const = 0;
    virtual void clear_attachment(uint32_t attachmentIndex, int value) = 0;
    virtual uint32_t get_color_attachment_id(uint32_t index = 0) const = 0;

    static std::shared_ptr<FrameBuffer> create(const FramebufferSpecification& spec);
};

} // namespace Kontomire

#endif // __KONTOMIRE_RENDERER_FRAMEBUFFER__