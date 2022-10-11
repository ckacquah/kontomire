#ifndef __OPENGL_CPP_TEMPLATE_LAYER__
#define __OPENGL_CPP_TEMPLATE_LAYER__

class Layer {
public:
    virtual void init() noexcept {};
    virtual void destroy() const noexcept {};
    virtual void update() const noexcept {};
};

#endif // __OPENGL_CPP_TEMPLATE_LAYER__