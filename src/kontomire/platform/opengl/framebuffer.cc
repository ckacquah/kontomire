#include <glad/glad.h>

#include "kontomire/platform/opengl/framebuffer.h"

namespace Kontomire {

static const uint32_t _MAX_FRAMEBUFFERSIZE = 8192;

namespace Utils {

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

    static void attach_color_texture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled) {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
    }

    static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;
        if (multisampled) {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        } else {
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
        switch (format) {
        case FramebufferTextureFormat::RGBA8:
            return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER:
            return GL_RED_INTEGER;
        default:
            return 0;
        }
    }

}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : _specification(spec)
{
    for (auto spec : _specification.attachments.attachments) {
        if (!Utils::is_depth_format(spec.texture_format))
            _color_attachment_specification.emplace_back(spec);
        else
            _depth_attachment_specification = spec;
    }

    invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &_id);
    glDeleteTextures(_color_attachment.size(), _color_attachment.data());
    glDeleteTextures(1, &_depth_attachment);
}

void OpenGLFramebuffer::invalidate()
{
    if (_id) {
        glDeleteFramebuffers(1, &_id);
        glDeleteTextures(_color_attachment.size(), _color_attachment.data());
        glDeleteTextures(1, &_depth_attachment);

        _color_attachment_specification.clear();
        _depth_attachment = 0;
    }

    glCreateFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    bool multisample = _specification.samples > 1;

    // Attachments
    if (_color_attachment_specification.size()) {
        _color_attachment.resize(_color_attachment_specification.size());
        Utils::create_texture(multisample, _color_attachment.data(), _color_attachment_specification.size());

        for (size_t i = 0; i < _color_attachment_specification.size(); i++) {
            Utils::bind_texture(multisample, _color_attachment[i]);
            switch (_color_attachment_specification[i].texture_format) {
            case FramebufferTextureFormat::RGBA8:
                Utils::attach_color_texture(_color_attachment[i], _specification.samples, GL_RGBA8, GL_RGBA, _specification.width, _specification.height, i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::attach_color_texture(_color_attachment[i], _specification.samples, GL_R32I, GL_RED_INTEGER, _specification.width, _specification.height, i);
                break;
            default:
                break;
            }
        }
    }

    if (_depth_attachment_specification.texture_format != FramebufferTextureFormat::None) {
        Utils::create_texture(multisample, &_depth_attachment, 1);
        Utils::bind_texture(multisample, _depth_attachment);
        if (_depth_attachment_specification.texture_format == FramebufferTextureFormat::DEPTH24STENCIL8) {
            Utils::attach_depth_texture(_depth_attachment, _specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, _specification.width, _specification.height);
        }
    }

    if (_color_attachment_specification.size() > 1) {
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(_color_attachment_specification.size(), buffers);
    } else if (_color_attachment_specification.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glViewport(0, 0, _specification.width, _specification.height);
}

void OpenGLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > _MAX_FRAMEBUFFERSIZE || height > _MAX_FRAMEBUFFERSIZE) {
        return;
    }
    _specification.width = width;
    _specification.height = height;

    invalidate();
}

int OpenGLFramebuffer::read_pixel(uint32_t attachmentIndex, int x, int y)
{

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLFramebuffer::clear_attachment(uint32_t attachmentIndex, int value)
{
    auto& spec = _color_attachment_specification[attachmentIndex];
    glClearTexImage(_color_attachment[attachmentIndex], 0,
        Utils::texture_format_to_GL(spec.texture_format), GL_INT, &value);
}

} // namespace Kontomire