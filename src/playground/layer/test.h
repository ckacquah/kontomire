#ifndef __KONTOMIRE_TEST_LAYER__
#define __KONTOMIRE_TEST_LAYER__

#include <vector>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include "layer.h"

class TestLayer : public Layer
{
  private:
    std::vector<int> bar_data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  public:
    void update() const override
    {
        ImGui::Begin("Implot Test");
        if (ImPlot::BeginPlot("My Plot"))
        {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_TEST_LAYER__