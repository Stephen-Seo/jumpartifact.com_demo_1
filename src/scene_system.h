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

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCENE_SYSTEM_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCENE_SYSTEM_H_

#include <bitset>
#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <optional>

// Forward declarations.
class SceneSystem;

class Scene {
 public:
  Scene(SceneSystem*);
  virtual ~Scene();

  virtual void update(SceneSystem* ctx, float dt) = 0;
  // Return true to DISALLOW drawing next scene.
  virtual bool draw(SceneSystem* ctx) = 0;

 private:
};

class SceneSystem {
 public:
  using SceneType = std::unique_ptr<Scene>;
  using SceneFnType = std::function<SceneType(SceneSystem*)>;
  using OptBuilderType = std::optional<SceneFnType>;

  using FlagsType = std::bitset<32>;

  SceneSystem();
  ~SceneSystem();

  // Disable copy.
  SceneSystem(const SceneSystem&) = delete;
  SceneSystem& operator=(const SceneSystem&) = delete;

  // Enable move.
  SceneSystem(SceneSystem&&) = default;
  SceneSystem& operator=(SceneSystem&&) = default;

  void update();
  void draw();

  void clear_scenes();
  void push_scene(SceneFnType scene_builder);
  void pop_scene();

  bool pop_was_queued() const;

  FlagsType& get_flags();
  const FlagsType& get_flags() const;

 private:
  enum class ActionType { CLEAR, PUSH, POP };
  struct Action {
    ActionType type;
    OptBuilderType scene_builder;
  };

  std::chrono::time_point<std::chrono::steady_clock> time_point;
  std::deque<SceneType> scene_stack;
  std::deque<Action> queued_actions;
  // 0 - is fullscreen
  FlagsType flags;
  // 0 - pop was queued, remains true until pop occurs
  std::bitset<32> private_flags;

  void handle_actions();
};

#endif
