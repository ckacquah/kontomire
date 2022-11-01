#ifndef __KONTOMIRE_LAYER__
#define __KONTOMIRE_LAYER__

class Layer
{
  public:
    virtual void init(){};
    virtual void destroy(){};
    virtual void update(){};
};

#endif // __KONTOMIRE_LAYER__