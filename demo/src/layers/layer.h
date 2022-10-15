#ifndef __OPENGL_CPP_TEMPLATE_LAYER__
#define __OPENGL_CPP_TEMPLATE_LAYER__

class Layer {
public:
    virtual void init() {};
    virtual void destroy() const {};
    virtual void update() const {};
};

#endif // __OPENGL_CPP_TEMPLATE_LAYER__