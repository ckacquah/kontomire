#ifndef __KONTOMIRE_PLAYGROUND_LAYER__
#define __KONTOMIRE_PLAYGROUND_LAYER__

#include <cstddef>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Layer
{
  public:
    virtual ~Layer() = default;

    virtual void init() = 0;
    virtual void render() = 0;
};

class ImGuiLayer : public Layer
{
  private:
    uint32_t width_{};
    uint32_t height_{};

    GLFWwindow* handle_{};

    std::string version_{};
    glm::vec4 clear_color_{glm::vec4(1.0f)};

  public:
    ImGuiLayer(const GLFWwindow* handle, const std::string& glsl_version = "#version 420");
    ~ImGuiLayer();

    virtual void init() override;
    virtual void render() override;

    void new_frame();
    void resize(uint32_t width, uint32_t height);
};

#endif // __KONTOMIRE_PLAYGROUND_LAYER__