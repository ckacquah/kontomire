#include <glad/glad.h>

#include "kontomire/platform/opengl/framebuffer.h"

namespace Kontomire
{

static const uint32_t _MAX_FRAMEBUFFERSIZE = 8192;

namespace Utils
{

static GLenum texture_target(bool multisampled)
{
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void create_texture(bool multisampled, uint32_t* out_id, uint32_t count)
{
    glCreateTextures(texture_target(multisampled), count, out_id);
}

static void bind_texture(bool multisampled, uint32_t id)
{
    glBindTexture(texture_target(multisampled), id);
}

static void attach_color_texture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width,
                                 uint32_t height, int index)
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
}

static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width,
                                 uint32_t height)
{
    bool multisampled = samples > 1;
    if (multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    }
    else
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, texture_target(multisampled), id, 0);
}

static bool is_depth_format(FramebufferTextureFormat format)
{
    return format == FramebufferTextureFormat::DEPTH24STENCIL8;
}

static GLenum texture_format_to_GL(FramebufferTextureFormat format)
{
    switch (format)
    {
    case FramebufferTextureFormat::RGBA8:
        return GL_RGBA8;
    case FramebufferTextureFormat::RED_INTEGER:
        return GL_RED_INTEGER;
    default:
        return 0;
    }
}

} // namespace Utils

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : specification_(spec)
{
    for (auto spec : specification_.attachments.attachments)
    {
        if (!Utils::is_depth_format(spec.texture_format))
            color_attachment_specification_.emplace_back(spec);
        else
            depth_attachment_specification_ = spec;
    }

    invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &id_);
    glDeleteTextures(color_attachment_.size(), color_attachment_.data());
    glDeleteTextures(1, &depth_attachment_);
}

void OpenGLFramebuffer::invalidate()
{
    if (id_)
    {
        glDeleteFramebuffers(1, &id_);
        glDeleteTextures(color_attachment_.size(), color_attachment_.data());
        glDeleteTextures(1, &depth_attachment_);

        color_attachment_specification_.clear();
        depth_attachment_ = 0;
    }

    glCreateFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);

    bool multisample = specification_.samples > 1;

    // Attachments
    if (color_attachment_specification_.size())
    {
        color_attachment_.resize(color_attachment_specification_.size());
        Utils::create_texture(multisample, color_attachment_.data(), color_attachment_specification_.size());

        for (size_t i = 0; i < color_attachment_specification_.size(); i++)
        {
            Utils::bind_texture(multisample, color_attachment_[i]);
            switch (color_attachment_specification_[i].texture_format)
            {
            case FramebufferTextureFormat::RGBA8:
                Utils::attach_color_texture(color_attachment_[i], specification_.samples, GL_RGBA8, GL_RGBA,
                                            specification_.width, specification_.height, i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::attach_color_texture(color_attachment_[i], specification_.samples, GL_R32I, GL_RED_INTEGER,
                                            specification_.width, specification_.height, i);
                break;
            default:
                break;
            }
        }
    }

    if (depth_attachment_specification_.texture_format != FramebufferTextureFormat::None)
    {
        Utils::create_texture(multisample, &depth_attachment_, 1);
        Utils::bind_texture(multisample, depth_attachment_);
        if (depth_attachment_specification_.texture_format == FramebufferTextureFormat::DEPTH24STENCIL8)
        {
            Utils::attach_depth_texture(depth_attachment_, specification_.samples, GL_DEPTH24_STENCIL8,
                                        GL_DEPTH_STENCIL_ATTACHMENT, specification_.width, specification_.height);
        }
    }

    if (color_attachment_specification_.size() > 1)
    {
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(color_attachment_specification_.size(), buffers);
    }
    else if (color_attachment_specification_.empty())
    {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glViewport(0, 0, specification_.width, specification_.height);
}

void OpenGLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > _MAX_FRAMEBUFFERSIZE || height > _MAX_FRAMEBUFFERSIZE)
    {
        return;
    }
    specification_.width = width;
    specification_.height = height;

    invalidate();
}

int OpenGLFramebuffer::read_pixel(uint32_t attachmentIndex, int x, int y)
{
    int pixelData;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLFramebuffer::clear_attachment(uint32_t attachmentIndex, int value)
{
    auto& spec = color_attachment_specification_[attachmentIndex];
    glClearTexImage(color_attachment_[attachmentIndex], 0, Utils::texture_format_to_GL(spec.texture_format), GL_INT,
                    &value);
}

} // namespace Kontomire