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
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

// Forward declarations.
class SceneSystem;

class Scene {
 public:
  Scene(SceneSystem*);
  virtual ~Scene();

  virtual void update(SceneSystem* ctx, float dt) = 0;
  virtual void draw(SceneSystem* ctx) = 0;
  virtual void draw_rlimgui(SceneSystem* ctx) = 0;

  // Return true to ALLOW drawing lower scenes on stack.
  virtual bool allow_draw_below(SceneSystem* ctx) = 0;

  std::optional<uint32_t> get_scene_id(SceneSystem* ctx);

 private:
};

class SceneSystem {
 public:
  using SceneType = std::unique_ptr<Scene>;
  using SceneFnType = std::function<SceneType(SceneSystem*)>;
  using OptBuilderType = std::optional<SceneFnType>;

  using MapType =
      std::unordered_map<std::string,
                         std::pair<void*, std::function<void(void*)> > >;

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

  const std::deque<SceneType>* get_scene_stack() const;
  std::optional<SceneType*> get_top();

  bool set_map_value(std::string name, void* value,
                     std::function<void(void*)> cleanup_fn);
  std::optional<void*> get_map_value(std::string name);
  bool clear_map_value(
      std::string name,
      std::optional<std::function<void(void*)> > override_cleanup_fn);

  std::optional<uint32_t> get_scene_id(Scene*);

  template <typename SceneTypeT>
  uint32_t get_scene_id_by_template();

  std::optional<uint32_t> get_top_scene_id();

 private:
  enum class ActionType { CLEAR, PUSH, POP };
  struct Action {
    ActionType type;
    OptBuilderType scene_builder;
  };

  std::chrono::time_point<std::chrono::steady_clock> time_point;
  std::deque<SceneType> scene_stack;
  std::deque<Action> queued_actions;
  MapType generic_map;
  std::array<float, 10> dt;
  size_t dt_idx;
  // 0 - is fullscreen
  // 1 - moonscript loaded
  FlagsType flags;
  // 0 - pop was queued, remains true until pop occurs
  // 1 - small font size
  // 2 - toggle font size
  // 3 - demo window open
  std::bitset<32> private_flags;
  std::unordered_map<std::string, uint32_t> scene_type_map;
  uint32_t scene_type_counter;

  void handle_actions();
};

template <typename SceneTypeT>
uint32_t SceneSystem::get_scene_id_by_template() {
  if (auto iter = scene_type_map.find(typeid(SceneTypeT).name());
      iter != scene_type_map.end()) {
    return iter->second;
  }

  scene_type_map.insert(
      std::make_pair(typeid(SceneTypeT).name(), scene_type_counter++));
  return scene_type_counter - 1;
}

#endif
