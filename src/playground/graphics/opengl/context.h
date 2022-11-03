#ifndef __KONTOMIRE_PLAYGROUND_OPENGL_CONTEXT__
#define __KONTOMIRE_PLAYGROUND_OPENGL_CONTEXT__

#include <GLFW/glfw3.h>

#include "../context.h"

class OpenGLContext : public GraphicsContext
{
  private:
    GLFWwindow* handle_;

  public:
    OpenGLContext(void* window) : handle_(static_cast<GLFWwindow*>(window))
    {
    }

    virtual ~OpenGLContext() = default;

    virtual void init() override;
    virtual void swap_buffers() override;
};

#endif // __KONTOMIRE_PLAYGROUND_OPENGL_CONTEXT__