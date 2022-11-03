#ifndef __KONTOMIRE_WINDOWS__
#define __KONTOMIRE_WINDOWS__

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <GLFW/glfw3.h>

#include "graphics/context.h"

using WindowSize = std::tuple<uint32_t, uint32_t>;

class Window
{
  public:
    enum class API
    {
        None,
        Glfw
    };

  private:
    static API api_;

  public:
    virtual ~Window() = default;

    virtual void update() = 0;
    virtual void destroy() = 0;

    virtual bool closed() const = 0;

    virtual WindowSize size() const = 0;

    virtual void* handle() const = 0;

    static std::unique_ptr<Window> create(const std::string& name, uint32_t width, uint32_t height);

    static Window::API api()
    {
        return api_;
    }
};

class GlfwWindow : public Window
{
  private:
    GLFWwindow* handle_{};
    std::unique_ptr<GraphicsContext> context_{};

  public:
    GlfwWindow(const std::string& name, uint32_t width, uint32_t height);
    ~GlfwWindow();

    virtual void update() override;
    virtual void destroy() override;

    virtual bool closed() const override;

    virtual WindowSize size() const override;

    virtual void* handle() const override
    {
        return handle_;
    };
};

#endif // __KONTOMIRE_WINDOWS__