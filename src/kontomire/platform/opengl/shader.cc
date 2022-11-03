#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kontomire/platform/opengl/shader.h"

namespace Knt
{

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
    : name_(name)
{
    shader_src_[GL_VERTEX_SHADER] = vertex_src;
    shader_src_[GL_FRAGMENT_SHADER] = fragment_src;
    create_program();
}

void OpenGLShader::create_program()
{
    id_ = glCreateProgram();

    compile_shaders();

    for (auto& shader : shaders_ids_)
    {
        glAttachShader(id_, shader);
    }

    glLinkProgram(id_);

    for (auto& shader : shaders_ids_)
    {
        glDeleteShader(shader);
    }

    shaders_ids_.clear();
}

const std::string& OpenGLShader::name() const
{
    return name_;
}

void OpenGLShader::compile_shaders()
{
    int success{};
    char logs[1024];

    for (auto&& [shader, shader_src] : shader_src_)
    {
        uint32_t shader_id = glCreateShader(shader);
        const char* shader_src_ = shader_src.c_str();

        glShaderSource(shader_id, 1, &shader_src_, NULL);
        glCompileShader(shader_id);
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, logs);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << logs << std::endl;
        }

        shaders_ids_.emplace_back(shader_id);
    }
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(id_);
}

void OpenGLShader::bind() const
{
    glUseProgram(id_);
}

void OpenGLShader::unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::set_int(const std::string& name, int value)
{
    upload_uniform_int(name, value);
}

void OpenGLShader::set_int_array(const std::string& name, int* values, uint32_t count)
{
    upload_uniform_int_array(name, values, count);
}

void OpenGLShader::set_float(const std::string& name, float value)
{
    upload_uniform_float(name, value);
}

void OpenGLShader::set_float2(const std::string& name, const glm::vec2& value)
{
    upload_uniform_float2(name, value);
}

void OpenGLShader::set_float3(const std::string& name, const glm::vec3& value)
{
    upload_uniform_float3(name, value);
}

void OpenGLShader::set_float4(const std::string& name, const glm::vec4& value)
{
    upload_uniform_float4(name, value);
}

void OpenGLShader::set_mat4(const std::string& name, const glm::mat4& value)
{
    upload_uniform_mat4(name, value);
}

void OpenGLShader::upload_uniform_int(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_int_array(const std::string& name, int* values, uint32_t count)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::upload_uniform_float(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float2(const std::string& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::upload_uniform_float3(const std::string& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::upload_uniform_float4(const std::string& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace Knt
