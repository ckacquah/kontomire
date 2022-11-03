#include <stdexcept>

#include <glad/glad.h>

#include "context.h"

void OpenGLContext::init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwMakeContextCurrent(handle_);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (!status)
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    if (GLVersion.major < 4 || (GLVersion.major != 4 && GLVersion.minor < 5))
    {
        throw std::runtime_error("Requires OpenGL version 4.5 or greater");
    }
}

void OpenGLContext::swap_buffers()
{
    glfwSwapBuffers(handle_);
}