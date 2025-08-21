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

#include "scene_system.h"

// third party includes
#ifndef NDEBUG
#include <emscripten/console.h>
#endif
#include <emscripten/html5.h>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

// standard library includes
#include <print>

// local includes
#include "test_lua_scene.h"

Scene::Scene(SceneSystem *) {}
Scene::~Scene() {}

uint32_t Scene::get_scene_id(SceneSystem *ctx) {
  return ctx->get_scene_id(this);
}

SceneSystem::SceneSystem()
    : scene_stack(),
      dt{1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F},
      dt_idx(0),
      flags(),
      private_flags(),
      scene_type_counter(0) {}

SceneSystem::~SceneSystem() {}

void SceneSystem::update() {
  auto next_time_point = std::chrono::steady_clock::now();
  float delta_time =
      (float)std::chrono::duration_cast<std::chrono::microseconds>(
          next_time_point - this->time_point)
          .count() /
      1000000.0F;
  this->time_point = std::move(next_time_point);

  dt[dt_idx++] = delta_time;
  if (dt_idx >= dt.size()) {
    dt_idx = 0;
  }

  handle_actions();

  for (auto iter = scene_stack.rbegin(); iter != scene_stack.rend(); ++iter) {
    (*iter)->update(this, delta_time);
  }
}

void SceneSystem::draw() {
  size_t disallow_below_idx = 0;
  for (size_t idx = 0; idx < scene_stack.size(); ++idx) {
    if (!scene_stack[idx]->allow_draw_below(this)) {
      disallow_below_idx = idx;
    }
  }

  for (size_t idx = scene_stack.size(); idx-- > disallow_below_idx;) {
    scene_stack[idx]->draw(this);
  }

  rlImGuiBegin();

  // Font size doubling.
  if (!private_flags.test(1)) {
    ImGui::PushFont(NULL, ImGui::GetFontSize() * 2.0F);
  }

  for (size_t idx = scene_stack.size(); idx-- > disallow_below_idx;) {
    scene_stack[idx]->draw_rlimgui(this);
  }

  if (private_flags.test(3)) {
    ImGui::ShowDemoWindow();
  }

  ImGui::Begin("Config Window");
  ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_None);
  if (ImGui::BeginTabItem("Settings")) {
    bool is_fullscreen = flags.test(0);
    ImGui::Checkbox("Fullscreen Enabled", &is_fullscreen);
    if (is_fullscreen != flags.test(0)) {
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
    flags.set(0, is_fullscreen);

    bool is_font_big = !private_flags.test(1);
    ImGui::Checkbox("Font Size Big", &is_font_big);
    if (is_font_big == private_flags.test(1)) {
      private_flags.set(2);
    }

    bool is_demo_window_open = private_flags.test(3);
    ImGui::Checkbox("Demo Window Active", &is_demo_window_open);
    if (is_demo_window_open != private_flags.test(3)) {
      private_flags.flip(3);
    }

    if (ImGui::Button("Clear Scenes")) {
      clear_scenes();
      std::println(stdout, "Cleared Scenes.");
    }

    if (ImGui::Button("Push \"Lua Test\" Scene")) {
      clear_scenes();
      push_scene(
          [](SceneSystem *ctx) { return std::make_unique<TestLuaScene>(ctx); });
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
  ImGui::End();

  // Font size doubling cleanup.
  if (!private_flags.test(1)) {
    ImGui::PopFont();
  }

  rlImGuiEnd();

  // Toggle doubling of font size.
  if (private_flags.test(2)) {
    private_flags.reset(2);
    private_flags.flip(1);
  }
}

void SceneSystem::clear_scenes() {
  queued_actions.emplace_back(ActionType::CLEAR, std::nullopt);
}

void SceneSystem::push_scene(SceneFnType scene_builder) {
  queued_actions.emplace_back(ActionType::PUSH, scene_builder);
}

void SceneSystem::pop_scene() {
  queued_actions.emplace_back(ActionType::POP, std::nullopt);
  private_flags.set(0);
}

bool SceneSystem::pop_was_queued() const { return private_flags.test(0); }

SceneSystem::FlagsType &SceneSystem::get_flags() { return flags; }

const SceneSystem::FlagsType &SceneSystem::get_flags() const { return flags; }

const std::deque<SceneSystem::SceneType> *SceneSystem::get_scene_stack() const {
  return &scene_stack;
}

std::optional<SceneSystem::SceneType *> SceneSystem::get_top() {
  if (scene_stack.empty()) {
    return std::nullopt;
  }
  return &scene_stack.back();
}

bool SceneSystem::set_map_value(std::string name, void *value,
                                std::function<void(void *)> cleanup_fn) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    return false;
  }
  generic_map.insert(std::make_pair(name, std::make_pair(value, cleanup_fn)));
  return true;
}

std::optional<void *> SceneSystem::get_map_value(std::string name) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    return iter->second.first;
  }
  return std::nullopt;
}

bool SceneSystem::clear_map_value(
    std::string name,
    std::optional<std::function<void(void *)> > override_cleanup_fn) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    if (override_cleanup_fn.has_value()) {
      override_cleanup_fn.value()(iter->second.first);
    } else {
      iter->second.second(iter->second.first);
    }
    generic_map.erase(iter);
    return true;
  }
  return false;
}

uint32_t SceneSystem::get_scene_id(Scene *scene) {
  if (auto iter = scene_type_map.find(typeid(scene).name());
      iter != scene_type_map.end()) {
    return iter->second;
  }

  scene_type_map.insert(
      std::make_pair(typeid(scene).name(), scene_type_counter++));
  return scene_type_counter - 1;
}

std::optional<uint32_t> SceneSystem::get_top_scene_id() {
  if (scene_stack.empty()) {
    return std::nullopt;
  }

  return get_scene_id(scene_stack.back().get());
}

void SceneSystem::handle_actions() {
  while (!queued_actions.empty()) {
    switch (queued_actions.front().type) {
      case ActionType::CLEAR:
        scene_stack.clear();
        break;
      case ActionType::PUSH:
        if (queued_actions.front().scene_builder) {
          scene_stack.push_back(
              queued_actions.front().scene_builder.value()(this));
        }
        break;
      case ActionType::POP:
        if (!scene_stack.empty()) {
          scene_stack.pop_back();
        }
        break;
    }
    queued_actions.pop_front();
  }
  private_flags.reset(0);
}
