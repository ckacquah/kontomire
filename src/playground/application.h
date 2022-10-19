#ifndef __KONTOMIRE_APPLICATION__
#define __KONTOMIRE_APPLICATION__

#include <memory>
#include <vector>

#include "layer/layer.h"
#include "layer/opengl.h"
#include "layer/test.h"
#include "window.h"

class Application
{
  private:
    std::unique_ptr<Window> window{};
    std::vector<std::unique_ptr<Layer>> layers{};

  public:
    Application();

    void run();
};

#endif // __KONTOMIRE_APPLICATION__