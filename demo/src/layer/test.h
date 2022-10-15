#ifndef __KONTOMIRE_TEST_LAYER__
#define __KONTOMIRE_TEST_LAYER__

#include <vector>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include "layer.h"

std::vector<int> bar_data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

class TestLayer1 : public Layer {
  public:
    void update() const noexcept override {
        ImGui::Begin("Testing 1 Layer");
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

class TestLayer2 : public Layer {
  public:
    void update() const noexcept override {
        ImGui::Begin("Testing 2 Layer");
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

class TestLayer3 : public Layer {
  public:
    void update() const noexcept override {
        ImGui::Begin("Testing 3 Layer");
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

class TestLayer4 : public Layer {
  public:
    void update() const noexcept override {
        ImGui::Begin("Testing 4 Layer");
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data.data(), 11);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }
};

#endif // __KONTOMIRE_TEST_LAYER__