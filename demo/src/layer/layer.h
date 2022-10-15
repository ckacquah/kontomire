#ifndef __KONTOMIRE_LAYER__
#define __KONTOMIRE_LAYER__

class Layer {
  public:
    virtual void init(){};
    virtual void destroy() const {};
    virtual void update() const {};
};

#endif // __KONTOMIRE_LAYER__