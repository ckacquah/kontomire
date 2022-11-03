#ifndef __KONTOMIRE_APPLICATION__
#define __KONTOMIRE_APPLICATION__

#include <memory>
#include <vector>

#include "layer/demo.h"
#include "layer/layer.h"
#include "window.h"

class Application
{
  private:
    std::unique_ptr<Window> window_{};
    std::unique_ptr<ImGuiLayer> imgui_layer_{};
    std::vector<std::unique_ptr<Layer>> layers_{};

  public:
    Application();

    void run();
};

#endif // __KONTOMIRE_APPLICATION__