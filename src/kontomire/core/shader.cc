#include "kontomire/core/shader.h"
#include "kontomire/core/renderAPI.h"
#include "kontomire/platform/opengl/shader.h"

namespace Kontomire
{

std::shared_ptr<Shader> Shader::create(const std::string& name, const std::string& vertex_src,
                                       const std::string& fragment_src)
{
    switch (RenderAPI::get_API())
    {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);
    }
    return nullptr;
}

void ShaderLibrary::add(const std::shared_ptr<Shader>& shader)
{
    add(shader->name(), shader);
}

void ShaderLibrary::add(const std::string& name, const std::shared_ptr<Shader>& shader)
{
    shaders_[name] = shader;
}

bool ShaderLibrary::exists(const std::string& name) const
{
    return shaders_.find(name) != shaders_.end();
}

} // namespace Kontomire
