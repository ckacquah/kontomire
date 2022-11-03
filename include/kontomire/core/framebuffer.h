#ifndef __KONTOMIRE_RENDERER_FRAMEBUFFER__
#define __KONTOMIRE_RENDERER_FRAMEBUFFER__

#include <memory>
#include <stdint.h>
#include <vector>

namespace Knt
{

enum class FramebufferTextureFormat
{
    NONE = 0,
    RGBA8,
    RED_INTEGER,
    DEPTH24STENCIL8,
    DEPTH = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureFormat texture_format{FramebufferTextureFormat::NONE};

    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format) : texture_format(format)
    {
    }
};

struct FramebufferAttachmentSpecification
{
    std::vector<FramebufferTextureSpecification> attachments{};

    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : attachments(attachments)
    {
    }
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
    virtual void clear_attachment(uint32_t index, int value) = 0;

    virtual int pixel(uint32_t index, int x, int y) = 0;

    virtual const FramebufferSpecification& specification() const = 0;

    virtual uint32_t color_attachment(uint32_t index = 0) const = 0;

    static std::shared_ptr<FrameBuffer> create(const FramebufferSpecification& spec);
};

} // namespace Knt

#endif // __KONTOMIRE_RENDERER_FRAMEBUFFER__