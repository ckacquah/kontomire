#ifndef __KONTOMIRE_CORE_SHADER__
#define __KONTOMIRE_CORE_SHADER__

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Kontomire
{

class Shader
{
  public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void set_int(const std::string& name, int value) = 0;
    virtual void set_int_array(const std::string& name, int* values, uint32_t count) = 0;

    virtual void set_float(const std::string& name, float value) = 0;
    virtual void set_float2(const std::string& name, const glm::vec2& value) = 0;
    virtual void set_float3(const std::string& name, const glm::vec3& value) = 0;
    virtual void set_float4(const std::string& name, const glm::vec4& value) = 0;

    virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;

    virtual const std::string& name() const = 0;

    static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertex_src,
                                          const std::string& fragment_src);
};

class ShaderLibrary
{
  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_;

  public:
    void add(const std::shared_ptr<Shader>& shader);
    void add(const std::string& name, const std::shared_ptr<Shader>& shader);

    std::shared_ptr<Shader> get(const std::string& name);

    bool exists(const std::string& name) const;
};

} // namespace Kontomire

#endif // __KONTOMIRE_CORE_SHADER__
