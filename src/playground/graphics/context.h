#ifndef __KONTOMIRE_PLAYGROUND_GRAPHIC_CONTEXT__
#define __KONTOMIRE_PLAYGROUND_GRAPHIC_CONTEXT__

#include <memory>

class GraphicsContext
{
  public:
    virtual ~GraphicsContext() = default;

    virtual void init() = 0;
    virtual void swap_buffers() = 0;

    static std::unique_ptr<GraphicsContext> create(void* window);
};

#endif // __KONTOMIRE_PLAYGROUND_GRAPHIC_CONTEXT__