// ISC License
//
// Copyright (c) 2025 Stephen Seo
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include "demo_scene.h"

// third-party includes
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

DemoScene::DemoScene(SceneSystem *ctx) : Scene(ctx), flags{}, dt_idx(0) {
  for (int idx = 0; idx < dt.size(); ++idx) {
    dt[idx] = 1.0F;
  }
}
DemoScene::~DemoScene() {}

void DemoScene::update(SceneSystem *ctx, float dt) {
  this->dt[dt_idx++] = dt;
  if (dt_idx >= this->dt.size()) {
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
    for (int idx = 0; idx < dt.size(); ++idx) {
      avg += dt[idx];
    }
    avg /= static_cast<float>(dt.size());
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
