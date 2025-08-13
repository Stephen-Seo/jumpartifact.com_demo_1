#include "demo_scene.h"

// third-party includes
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

DemoScene::DemoScene() {}
DemoScene::~DemoScene() {}

void DemoScene::update(SceneSystem *ctx, float dt) {}

bool DemoScene::draw(SceneSystem *ctx) {
  rlImGuiBegin();

  // Double the font size.
  ImGui::PushFont(NULL, ImGui::GetFontSize() * 2.0F);

  bool open = true;
  ImGui::ShowDemoWindow(&open);

  open = true;
  if (ImGui::Begin("Test Window", &open)) {
    ImGui::Text("Test text in the Test Window.");
  }

  // Cleanup of doubling the font size.
  ImGui::PopFont();

  ImGui::End();

  rlImGuiEnd();
  return false;
}
