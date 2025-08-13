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

Scene::Scene() {}
Scene::~Scene() {}

SceneSystem::SceneSystem() : scene_stack() {}

SceneSystem::~SceneSystem() {}

void SceneSystem::update() {
  auto next_time_point = std::chrono::steady_clock::now();
  float delta_time =
      (float)std::chrono::duration_cast<std::chrono::microseconds>(
          next_time_point - this->time_point)
          .count() /
      1000000.0F;
  this->time_point = std::move(next_time_point);

  for (auto iter = scene_stack.rbegin(); iter != scene_stack.rend(); ++iter) {
    (*iter)->update(this, delta_time);
  }

  handle_actions();
}

void SceneSystem::draw() {
  for (auto iter = scene_stack.rbegin(); iter != scene_stack.rend(); ++iter) {
    if (!(*iter)->draw(this)) {
      break;
    }
  }
}

void SceneSystem::clear_scenes() {
  queued_actions.emplace_back(ActionType::CLEAR, std::nullopt);
}

void SceneSystem::push_scene(
    std::function<SceneSystem::SceneType()> scene_builder) {
  queued_actions.emplace_back(ActionType::PUSH, scene_builder);
}

void SceneSystem::pop_scene() {
  queued_actions.emplace_back(ActionType::POP, std::nullopt);
}

void SceneSystem::handle_actions() {
  while (!queued_actions.empty()) {
    switch (queued_actions.front().type) {
      case ActionType::CLEAR:
        scene_stack.clear();
        break;
      case ActionType::PUSH:
        if (queued_actions.front().scene_builder) {
          scene_stack.push_back(queued_actions.front().scene_builder.value()());
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
}
