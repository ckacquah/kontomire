#ifndef __KONTOMIRE_OPENGL_SHADER__
#define __KONTOMIRE_OPENGL_SHADER__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "kontomire/core/shader.h"

namespace knt
{

class OpenGLShader : public Shader
{
  private:
    uint32_t id_;
    std::string name_;
    std::string filepath_;

    std::vector<uint32_t> shaders_ids_;
    std::unordered_map<uint32_t, std::string> shader_src_;

  private:
    void create_program();
    void compile_shaders();

    void upload_uniform_int(const std::string& name, int value);
    void upload_uniform_int_array(const std::string& name, int* values, uint32_t count);

    void upload_uniform_float(const std::string& name, float value);
    void upload_uniform_float2(const std::string& name, const glm::vec2& value);
    void upload_uniform_float3(const std::string& name, const glm::vec3& value);
    void upload_uniform_float4(const std::string& name, const glm::vec4& value);

    void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix);

  public:
    OpenGLShader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
    ~OpenGLShader();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void set_int(const std::string& name, int value) override;
    virtual void set_int_array(const std::string& name, int* values, uint32_t count) override;

    virtual void set_float(const std::string& name, float value) override;
    virtual void set_float2(const std::string& name, const glm::vec2& value) override;
    virtual void set_float3(const std::string& name, const glm::vec3& value) override;
    virtual void set_float4(const std::string& name, const glm::vec4& value) override;

    virtual void set_mat4(const std::string& name, const glm::mat4& value) override;

    virtual const std::string& name() const override;
};

} // namespace knt

#endif // __KONTOMIRE_OPENGL_SHADER__
