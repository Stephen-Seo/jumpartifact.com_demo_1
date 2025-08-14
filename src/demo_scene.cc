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
#include <emscripten/html5.h>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

// standard library includes
#include <print>

DemoScene::DemoScene(SceneSystem *ctx) : Scene(ctx), flags{}, dt_idx(0) {
  for (int idx = 0; idx < dt.size(); ++idx) {
    dt[idx] = 1.0F;
  }
  // demo window open
  flags.set(2);
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

  if (flags.test(2)) {
    ImGui::ShowDemoWindow();
  }

  if (ImGui::Begin("Config Window")) {
    if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_None)) {
      if (ImGui::BeginTabItem("Settings")) {
        bool is_fullscreen = ctx->get_flags().test(0);
        ImGui::Checkbox("Fullscreen Enabled", &is_fullscreen);
        if (is_fullscreen != ctx->get_flags().test(0)) {
          if (is_fullscreen) {
            if (emscripten_request_fullscreen("canvas", true) !=
                EMSCRIPTEN_RESULT_SUCCESS) {
              is_fullscreen = false;
            }
          } else {
            if (emscripten_exit_fullscreen() != EMSCRIPTEN_RESULT_SUCCESS) {
              is_fullscreen = true;
            }
          }
        }
        ctx->get_flags().set(0, is_fullscreen);

        bool is_font_big = !flags.test(0);
        ImGui::Checkbox("Font Size Big", &is_font_big);
        if (is_font_big == flags.test(0)) {
          flags.set(1);
        }

        bool is_demo_window_open = flags.test(2);
        ImGui::Checkbox("Demo Window Active", &is_demo_window_open);
        if (is_demo_window_open != flags.test(2)) {
          flags.flip(2);
        }

        if (ImGui::Button("Reset")) {
          ctx->clear_scenes();
          std::println(stdout, "Reset Scenes.");
        }

        float avg = 0.0F;
        for (int idx = 0; idx < dt.size(); ++idx) {
          avg += dt[idx];
        }
        avg /= static_cast<float>(dt.size());
        ImGui::Text("Current FPS is: %0.1f", 1.0F / avg);

        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::End();
  }

  // Cleanup of doubling the font size.
  if (!flags.test(0)) {
    ImGui::PopFont();
  }

  rlImGuiEnd();

  if (flags.test(1)) {
    flags.reset(1);
    flags.flip(0);
  }
  return false;
}
