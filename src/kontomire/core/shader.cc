#include <fstream>
#include <iostream>
#include <map>

#include "kontomire/core/renderAPI.h"
#include "kontomire/core/shader.h"
#include "kontomire/platform/opengl/shader.h"

namespace Kontomire
{

namespace ShaderUtils
{

std::string read_file(const std::string& path)
{
    std::string result;
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1)
        {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        }
        else
        {
            throw std::runtime_error("Could not read from file " + path);
        }
    }
    else
    {
        throw std::runtime_error("Could not read from file " + path);
    }
    return result;
}

std::pair<std::string, std::string> pre_process_source(const std::string& source)
{
    std::pair<std::string, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
        if (eol == std::string::npos)
        {
            throw std::runtime_error("Shader Syntax error");
        }

        size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);

        if (eol == std::string::npos)
        {
            throw std::runtime_error("Invalid shader type specified");
        }

        size_t nextLinePos =
            source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
        if (nextLinePos == std::string::npos)
        {
            throw std::runtime_error("Shader Syntax error");
        }
        pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

        auto src =
            (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);

        if (type == "vertex")
        {
            shaderSources.first = src;
        }
        else if (type == "fragment")
        {
            shaderSources.second = src;
        }
    }

    return shaderSources;
}

} // namespace ShaderUtils

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

std::shared_ptr<Shader> Shader::create(const std::string& path)
{
    switch (RenderAPI::get_API())
    {
    case RenderAPI::API::None:
        return nullptr;
    case RenderAPI::API::OpenGL:
        std::string shader_src = ShaderUtils::read_file(path);
        auto shaders = ShaderUtils::pre_process_source(shader_src);
        return std::make_shared<OpenGLShader>(path, shaders.first, shaders.second);
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
