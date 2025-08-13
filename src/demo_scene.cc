#include "demo_scene.h"

// third-party includes
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

DemoScene::DemoScene() : flags{}, dt_idx(0) {
  for (int idx = 0; idx < DT_ARR_SIZE; ++idx) {
    dt[idx] = 1.0F;
  }
}
DemoScene::~DemoScene() {}

void DemoScene::update(SceneSystem *ctx, float dt) {
  this->dt[dt_idx++] = dt;
  if (dt_idx >= DT_ARR_SIZE) {
    dt_idx = 0;
  }
}

bool DemoScene::draw(SceneSystem *ctx) {
  rlImGuiBegin();

  // Double the font size.
  if (!flags.test(0)) {
    ImGui::PushFont(NULL, ImGui::GetFontSize() * 2.0F);
  }

  bool open = true;
  ImGui::ShowDemoWindow(&open);

  open = true;
  if (ImGui::Begin("Test Window", &open)) {
    if (ImGui::Button("Toggle Font Size")) {
      flags.set(1);
    }

    float avg = 0.0F;
    for (int idx = 0; idx < DT_ARR_SIZE; ++idx) {
      avg += dt[idx];
    }
    avg /= static_cast<float>(DT_ARR_SIZE);
    ImGui::Text("Current FPS is: %0.1f", 1.0F / avg);
  }

  // Cleanup of doubling the font size.
  if (!flags.test(0)) {
    ImGui::PopFont();
  }

  ImGui::End();

  rlImGuiEnd();

  if (flags.test(1)) {
    flags.reset(1);
    flags.flip(0);
  }
  return false;
}
